/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qSlicerSubjectHierarchyTransformsPlugin_h
#define __qSlicerSubjectHierarchyTransformsPlugin_h

// SubjectHierarchy includes
#include "qSlicerSubjectHierarchyAbstractPlugin.h"

#include "qSlicerTransformsSubjectHierarchyPluginsExport.h"

class qSlicerSubjectHierarchyTransformsPluginPrivate;

// Due to some reason the Python wrapping of this class fails, therefore
// put everything between BTX/ETX to exclude from wrapping.
// TODO investigate why the wrapping fails:
//   https://www.assembla.com/spaces/slicerrt/tickets/210-python-wrapping-error-when-starting-up-slicer-with-slicerrt
//BTX

/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class Q_SLICER_TRANSFORMS_SUBJECT_HIERARCHY_PLUGINS_EXPORT qSlicerSubjectHierarchyTransformsPlugin : public qSlicerSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qSlicerSubjectHierarchyAbstractPlugin Superclass;
  qSlicerSubjectHierarchyTransformsPlugin(QObject* parent = NULL);
  virtual ~qSlicerSubjectHierarchyTransformsPlugin();

public:
  /// Determines if a subject hierarchy item can be reparented in the hierarchy using the current plugin,
  /// and gets a confidence value for the reparented item.
  /// Most plugins do not perform steps additional to the default, so the default implementation returns a 0
  /// confidence value, which can be overridden in plugins that do handle special cases.
  /// \param itemID Item to be reparented in the hierarchy
  /// \param parentItemID Prospective parent of the item to reparent.
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   item, and 1 means that the plugin is the only one that can handle the item
  virtual double canReparentItemInsideSubjectHierarchy(vtkIdType itemID, vtkIdType parentItemID)const;

  /// Reparent an item that was already in the subject hierarchy under a new parent.
  /// \return True if reparented successfully, false otherwise
  virtual bool reparentItemInsideSubjectHierarchy(vtkIdType itemID, vtkIdType parentItemID);

  /// Determines if a data node can be placed in the hierarchy using the actual plugin,
  /// and gets a confidence value for a certain MRML node (usually the type and possibly attributes are checked).
  /// \param node Node to be added to the hierarchy
  /// \param parentItemID Prospective parent of the node to add.
  ///   Default value is invalid. In that case the parent will be ignored, the confidence numbers are got based on the to-be child node alone.
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   node, and 1 means that the plugin is the only one that can handle the node (by node type or identifier attribute)
  virtual double canAddNodeToSubjectHierarchy(
    vtkMRMLNode* node,
    vtkIdType parentItemID=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID )const;

  /// Determines if the actual plugin can handle a subject hierarchy item. The plugin with
  /// the highest confidence number will "own" the item in the subject hierarchy (set icon, tooltip,
  /// set context menu etc.)
  /// \param item Item to handle in the subject hierarchy tree
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   item, and 1 means that the plugin is the only one that can handle the item (by node type or identifier attribute)
  virtual double canOwnSubjectHierarchyItem(vtkIdType itemID)const;

  /// Get role that the plugin assigns to the subject hierarchy item.
  ///   Each plugin should provide only one role.
  Q_INVOKABLE virtual const QString roleForPlugin()const;

  /// Get icon of an owned subject hierarchy item
  /// \return Icon to set, empty icon if nothing to set
  virtual QIcon icon(vtkIdType itemID);

  /// Get visibility icon for a visibility state
  virtual QIcon visibilityIcon(int visible);

  /// Generate tooltip for a owned subject hierarchy item
  virtual QString tooltip(vtkIdType itemID)const;

  /// Get item context menu item actions to add to tree view
  virtual QList<QAction*> itemContextMenuActions()const;

  /// Show context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the context menu items for
  virtual void showContextMenuActionsForItem(vtkIdType itemID);

protected slots:
  /// Invert selected transform
  void invert();

  /// Set transform to identity (only available for linear transforms)
  void identity();

protected:
  QScopedPointer<qSlicerSubjectHierarchyTransformsPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerSubjectHierarchyTransformsPlugin);
  Q_DISABLE_COPY(qSlicerSubjectHierarchyTransformsPlugin);
};

//ETX

#endif