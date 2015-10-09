/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// PairwiseMetrics Logic includes
#include "vtkSlicerPairwiseMetricsLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageLogic.h>
#include <vtkIntArray.h>
#include <vtkMultiThreader.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPairwiseMetricsLogic);

std::vector<const vtkMRMLLabelMapVolumeNode*> vtkSlicerPairwiseMetricsLogic::LabelMaps;
double** vtkSlicerPairwiseMetricsLogic::ResultsArray;

//----------------------------------------------------------------------------
vtkSlicerPairwiseMetricsLogic::vtkSlicerPairwiseMetricsLogic()
{
  this->Threader = vtkMultiThreader::New();
}

//----------------------------------------------------------------------------
vtkSlicerPairwiseMetricsLogic::~vtkSlicerPairwiseMetricsLogic()
{
  this->Threader->Delete();
  this->Threader = NULL;
}

//----------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic
::ClearLabelMaps()
{
  this->LabelMaps.clear();
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic
::AddLabelMap(const vtkMRMLLabelMapVolumeNode* labelmap)
{
  if (labelmap != NULL)
    {
    this->LabelMaps.push_back(labelmap);
    }
}

//---------------------------------------------------------------------------
void vtkSlicerPairwiseMetricsLogic
::CalculateDiceCoefficients()
{
  if (!this->Threader)
    {
    return;
    }

  // Initialize results array
  // TODO: Lock it
  this->ResultsArray = new double*[this->LabelMaps.size()];
  for (unsigned int i = 0; i < this->LabelMaps.size(); ++i)
    {
    this->ResultsArray[i] = new double[this->LabelMaps.size()];
    }

  this->Threader->SetSingleMethod((vtkThreadFunctionType)&vtkSlicerPairwiseMetricsLogic::ThreadedDice,this);
  this->Threader->SingleMethodExecute();
}

//---------------------------------------------------------------------------
VTK_THREAD_RETURN_TYPE vtkSlicerPairwiseMetricsLogic::
ThreadedDice(void* arg)
{
  vtkSlicerPairwiseMetricsLogic* pmLogic =
    static_cast<vtkSlicerPairwiseMetricsLogic*>(arg);
  if (!pmLogic)
    {
    return VTK_THREAD_RETURN_VALUE;
    }

  int numberOfLabelMaps = pmLogic->LabelMaps.size();
  for (int i = 0; i < numberOfLabelMaps; ++i)
    {
    for (int j = i; j < numberOfLabelMaps; ++j)
      {
      if (i == j)
	{
	pmLogic->ResultsArray[i][j] = 1.0;
	continue;
	}

      // Remove the const of labelmaps here otherwise cannot access image data
      vtkMRMLLabelMapVolumeNode* labelMap1 = const_cast<vtkMRMLLabelMapVolumeNode*>(pmLogic->LabelMaps[i]);
      vtkMRMLLabelMapVolumeNode* labelMap2 = const_cast<vtkMRMLLabelMapVolumeNode*>(pmLogic->LabelMaps[j]);
      if (labelMap1 == NULL || labelMap2 == NULL)
	{
	return VTK_THREAD_RETURN_VALUE;
	}

      int pixelLabelMap1 = pmLogic->GetNumberOfPixels(labelMap1->GetImageData());
      int pixelLabelMap2 = pmLogic->GetNumberOfPixels(labelMap2->GetImageData());

      if (pixelLabelMap1 > 0 && pixelLabelMap2 > 0)
	{
	int pixelIntersection = pmLogic->CalculateIntersection(labelMap1->GetImageData(),
							       labelMap2->GetImageData());

	if (pixelIntersection >= 0)
	  {
	  double diceCoefficient = 2.0*pixelIntersection / (pixelLabelMap1 + pixelLabelMap2);
	  pmLogic->ResultsArray[i][j] = pmLogic->ResultsArray[j][i] = diceCoefficient;
	  }
	}
      }
    }

  return VTK_THREAD_RETURN_VALUE;
}

//---------------------------------------------------------------------------
int vtkSlicerPairwiseMetricsLogic
::GetNumberOfPixels(vtkImageData* imData)
{
  if (!imData)
    {
    return -1;
    }

  int numberOfPixels = 0;
  vtkSmartPointer<vtkImageAccumulate> pixelCounter
    = vtkSmartPointer<vtkImageAccumulate>::New();
#if VTK_MAJOR_VERSION <= 5
  pixelCounter->SetInput(imData);
#else
  pixelCounter->SetInputData(imData);
#endif
  pixelCounter->IgnoreZeroOn();
  pixelCounter->Update();

  numberOfPixels = pixelCounter->GetVoxelCount();

  return numberOfPixels;
}

//---------------------------------------------------------------------------
int vtkSlicerPairwiseMetricsLogic
::CalculateIntersection(vtkImageData* imData1, vtkImageData* imData2)
{
  if (!imData1 || !imData2)
    {
    return -1;
    }

  int numberOfCommonPixels = 0;

  vtkSmartPointer<vtkImageLogic> logicFilter
    = vtkSmartPointer<vtkImageLogic>::New();
#if VTK_MAJOR_VERSION <= 5
  logicFilter->SetInput1(imData1);
  logicFilter->SetInput2(imData2);
#else
  logicFilter->SetInput1Data(imData1);
  logicFilter->SetInput2Data(imData2);
#endif
  logicFilter->SetOperationToAnd();
  logicFilter->Update();

  numberOfCommonPixels = this->GetNumberOfPixels(logicFilter->GetOutput());

  return numberOfCommonPixels;
}
