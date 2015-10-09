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

// Qt includes
#include <QtPlugin>

// PairwiseMetrics Logic includes
#include <vtkSlicerPairwiseMetricsLogic.h>

// PairwiseMetrics includes
#include "qSlicerPairwiseMetricsModule.h"
#include "qSlicerPairwiseMetricsModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPairwiseMetricsModule, qSlicerPairwiseMetricsModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPairwiseMetricsModulePrivate
{
public:
  qSlicerPairwiseMetricsModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPairwiseMetricsModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModulePrivate::qSlicerPairwiseMetricsModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPairwiseMetricsModule methods

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModule::qSlicerPairwiseMetricsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPairwiseMetricsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModule::~qSlicerPairwiseMetricsModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPairwiseMetricsModule::helpText() const
{
  return "This extension compute pairwise metrics such as Dice coefficient, Generale Hausdorff distance and Average Minimum Distance using multiple threads";
}

//-----------------------------------------------------------------------------
QString qSlicerPairwiseMetricsModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPairwiseMetricsModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Laurent Chauvin (BWH), Sonia Pujol (BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPairwiseMetricsModule::icon() const
{
  return QIcon(":/Icons/PairwiseMetrics.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPairwiseMetricsModule::categories() const
{
  return QStringList() << "Quantification";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPairwiseMetricsModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerPairwiseMetricsModule
::createWidgetRepresentation()
{
  return new qSlicerPairwiseMetricsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPairwiseMetricsModule::createLogic()
{
  return vtkSlicerPairwiseMetricsLogic::New();
}
