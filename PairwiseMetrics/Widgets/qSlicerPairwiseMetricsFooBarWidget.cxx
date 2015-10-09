/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerPairwiseMetricsFooBarWidget.h"
#include "ui_qSlicerPairwiseMetricsFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PairwiseMetrics
class qSlicerPairwiseMetricsFooBarWidgetPrivate
  : public Ui_qSlicerPairwiseMetricsFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerPairwiseMetricsFooBarWidget);
protected:
  qSlicerPairwiseMetricsFooBarWidget* const q_ptr;

public:
  qSlicerPairwiseMetricsFooBarWidgetPrivate(
    qSlicerPairwiseMetricsFooBarWidget& object);
  virtual void setupUi(qSlicerPairwiseMetricsFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerPairwiseMetricsFooBarWidgetPrivate
::qSlicerPairwiseMetricsFooBarWidgetPrivate(
  qSlicerPairwiseMetricsFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerPairwiseMetricsFooBarWidgetPrivate
::setupUi(qSlicerPairwiseMetricsFooBarWidget* widget)
{
  this->Ui_qSlicerPairwiseMetricsFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPairwiseMetricsFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsFooBarWidget
::qSlicerPairwiseMetricsFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPairwiseMetricsFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerPairwiseMetricsFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPairwiseMetricsFooBarWidget
::~qSlicerPairwiseMetricsFooBarWidget()
{
}
