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

// .NAME vtkSlicerPairwiseMetricsLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerPairwiseMetricsLogic_h
#define __vtkSlicerPairwiseMetricsLogic_h

// VTK includes
#include <vtkImageData.h>
#include <vtkMultiThreader.h>

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include "vtkMRMLLabelMapVolumeNode.h"

// STD includes
#include <cstdlib>

#include "vtkSlicerPairwiseMetricsModuleLogicExport.h"

class vtkImageData;
class vtkMultiThreader;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_PAIRWISEMETRICS_MODULE_LOGIC_EXPORT vtkSlicerPairwiseMetricsLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerPairwiseMetricsLogic *New();
  vtkTypeMacro(vtkSlicerPairwiseMetricsLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  void ClearLabelMaps();
  void AddLabelMap(const vtkMRMLLabelMapVolumeNode* labelmap);
  void CalculateDiceCoefficients();

protected:
  vtkSlicerPairwiseMetricsLogic();
  virtual ~vtkSlicerPairwiseMetricsLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);

  VTK_THREAD_RETURN_TYPE ThreadedDice(void* arg);
  int GetNumberOfPixels(vtkImageData* imData);
  int CalculateIntersection(vtkImageData* label1, vtkImageData* label2);

private:

  vtkSlicerPairwiseMetricsLogic(const vtkSlicerPairwiseMetricsLogic&); // Not implemented
  void operator=(const vtkSlicerPairwiseMetricsLogic&); // Not implemented

  static std::vector<const vtkMRMLLabelMapVolumeNode*> LabelMaps;
  static double** ResultsArray;
  vtkMultiThreader* Threader;

};

#endif
