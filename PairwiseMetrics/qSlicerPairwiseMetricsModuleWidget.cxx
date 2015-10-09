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
#include <QDebug>

// SlicerQt includes
#include "qMRMLNodeComboBox.h"

#include "vtkSlicerPairwiseMetricsLogic.h"

#include "vtkMRMLLabelMapVolumeNode.h"

#include "qSlicerPairwiseMetricsModuleWidget.h"
#include "ui_qSlicerPairwiseMetricsModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPairwiseMetricsModuleWidgetPrivate: public Ui_qSlicerPairwiseMetricsModuleWidget
{
public:
  qSlicerPairwiseMetricsModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPairwiseMetricsModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModuleWidgetPrivate::qSlicerPairwiseMetricsModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPairwiseMetricsModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModuleWidget::qSlicerPairwiseMetricsModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPairwiseMetricsModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsModuleWidget::~qSlicerPairwiseMetricsModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::setup()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  connect(this, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
	  this, SLOT(onMRMLSceneChanged(vtkMRMLScene*)));
  connect(d->NumberOfItemsWidgets, SIGNAL(valueChanged(int)),
	  this, SLOT(onNumberOfWidgetsChanged(int)));
  connect(d->DiceButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMetricChanged()));
  connect(d->HausdorffButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMetricChanged()));
  connect(d->AMDButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMetricChanged()));
  connect(d->ComputeButton, SIGNAL(clicked()),
	  this, SLOT(onComputeClicked()));
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::onMRMLSceneChanged(vtkMRMLScene* vtkNotUsed(scene))
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  d->NumberOfItemsWidgets->setValue(2);
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::onNumberOfWidgetsChanged(int nWidgets)
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  int currentNumberWidgets = d->WidgetLayout->count();

  if (currentNumberWidgets == nWidgets)
    {
    return;
    }

  if (currentNumberWidgets > nWidgets)
    {
    // Remove (currentNumberWidgets - nWidgets) widgets
    for (int i = nWidgets; i < currentNumberWidgets; ++i)
      {
      // Safe way to remove item from layout (from Qt doc)
      QLayoutItem* child;
      if ((child = d->WidgetLayout->takeAt(i)) != 0)
	{
	d->WidgetLayout->removeWidget(child->widget());
	child->widget()->deleteLater();
	delete child;
	}
      }
    }
  else
    {
    // Add (nWidgets - currentNumberWidgets) widgets
    for (int i = currentNumberWidgets; i < nWidgets; ++i)
      {
      qMRMLNodeComboBox* newItem = new qMRMLNodeComboBox(d->CTKCollapsibleButton);
      if (newItem)
	{
	newItem->setMRMLScene(this->mrmlScene());
	newItem->setNodeTypes(QStringList() << (d->DiceButton->isChecked() ?
						"vtkMRMLLabelMapVolumeNode" :
						"vtkMRMLModelNode"));
	d->WidgetLayout->addWidget(newItem);
	}
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::onMetricChanged()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  // Change widgets node selection
  for (int i = 0; i < d->WidgetLayout->count(); ++i)
    {
    QLayoutItem* child;
    if ((child = d->WidgetLayout->itemAt(i)) != 0)
      {
      qMRMLNodeComboBox* item = qobject_cast<qMRMLNodeComboBox*>(child->widget());
      item->setNodeTypes(QStringList() << (d->DiceButton->isChecked() ?
					   "vtkMRMLLabelMapVolumeNode" :
					   "vtkMRMLModelNode"));
      }
    }

  // Change button text
  if (d->DiceButton->isChecked())
    {
    d->ComputeButton->setText("Compute Dice");
    }
  else if (d->HausdorffButton->isChecked())
    {
    d->ComputeButton->setText("Compute Hausdorff");
    }
  else if (d->AMDButton->isChecked())
    {
    d->ComputeButton->setText("Compute AMD");
    }
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::onComputeClicked()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  if (d->DiceButton->isChecked())
    {
    this->computeDiceMetric();
    }
  else if (d->HausdorffButton->isChecked())
    {
    this->computeHausdorffMetric();
    }
  else if (d->AMDButton->isChecked())
    {
    this->computeAMDMetric();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::computeDiceMetric()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  vtkSlicerPairwiseMetricsLogic* pmLogic =
    vtkSlicerPairwiseMetricsLogic::SafeDownCast(this->logic());
  if (!pmLogic)
    {
    return;
    }

  this->setLabelMapToLogic();
  pmLogic->CalculateDiceCoefficients();
}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::computeHausdorffMetric()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::computeAMDMetric()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

}

//-----------------------------------------------------------------------------
void qSlicerPairwiseMetricsModuleWidget::setLabelMapToLogic()
{
  Q_D(qSlicerPairwiseMetricsModuleWidget);

  vtkSlicerPairwiseMetricsLogic* pmLogic =
    vtkSlicerPairwiseMetricsLogic::SafeDownCast(this->logic());
  if (!pmLogic)
    {
    return;
    }

  pmLogic->ClearLabelMaps();
  for (int i = 0; i < d->WidgetLayout->count(); ++i)
    {
    QLayoutItem* child;
    if ((child = d->WidgetLayout->itemAt(i)) != 0)
      {
      qMRMLNodeComboBox* widget = qobject_cast<qMRMLNodeComboBox*>(child->widget());
      if (widget)
	{
	vtkMRMLLabelMapVolumeNode* labelmap =
	  vtkMRMLLabelMapVolumeNode::SafeDownCast(widget->currentNode());
	if (labelmap)
	  {
	  pmLogic->AddLabelMap(labelmap);
	  }
	}
      }
    }
}
