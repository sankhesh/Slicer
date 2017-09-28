/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkHyperTreeGridAlgorithm.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkHyperTreeGridAlgorithm
 * @brief   Superclass for algorithms that produce
 * a hyper tree grid as output
 *
 *
 * vtkHyperTreeGridAlgorithm is a base class for hyper tree grid algorithms.
 * This class defaults with one input port and one output port; it must be
 * modified by the concrete derived class if a different behavior is sought.
 * In addition, this class provides a FillOutputPortInfo() method that, by
 * default, specifies that the output is a data object; this
 * must also be modified in concrete subclasses if needed.
 *
 * @par Thanks:
 * This test was written by Philippe Pebay and Charles Law, Kitware 2012
 * This test was rewritten by Philippe Pebay, 2016
 * This work was supported by Commissariat a l'Energie Atomique (CEA/DIF)
*/

#ifndef vtkHyperTreeGridAlgorithm_h
#define vtkHyperTreeGridAlgorithm_h

#include "vtkCommonExecutionModelModule.h" // For export macro
#include "vtkAlgorithm.h"

class vtkBitArray;
class vtkDataSetAttributes;
class vtkHyperTreeGrid;
class vtkPolyData;
class vtkUnstructuredGrid;

class VTKCOMMONEXECUTIONMODEL_EXPORT vtkHyperTreeGridAlgorithm : public vtkAlgorithm
{
public:
  vtkTypeMacro(vtkHyperTreeGridAlgorithm,vtkAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  //@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  vtkDataObject* GetOutput();
  vtkDataObject* GetOutput( int );
  virtual void SetOutput( vtkDataObject* );
  //@}

  //@{
  /**
   * Get the output as a hyper tree grid.
   */
  vtkHyperTreeGrid* GetHyperTreeGridOutput();
  vtkHyperTreeGrid* GetHyperTreeGridOutput( int );
  //@}

  //@{
  /**
   * Get the output as a polygonal dataset.
   */
  vtkPolyData* GetPolyDataOutput();
  vtkPolyData* GetPolyDataOutput( int );
  //@}

  //@{
  /**
   * Get the output as an unstructured grid.
   */
  vtkUnstructuredGrid* GetUnstructuredGridOutput();
  vtkUnstructuredGrid* GetUnstructuredGridOutput( int );
  //@}

  /**
   * See vtkAlgorithm for details
   */
  virtual int ProcessRequest( vtkInformation*,
                              vtkInformationVector**,
                              vtkInformationVector*) VTK_OVERRIDE;

  //@{
  /**
   * Assign a data object as input. Note that this method does not
   * establish a pipeline connection. Use SetInputConnection() to
   * setup a pipeline connection.
   */
  void SetInputData( vtkDataObject* );
  void SetInputData( int, vtkDataObject* );
  //@}

  //@{
  /**
   * Assign a data object as input. Note that this method does not
   * establish a pipeline connection. Use AddInputConnection() to
   * setup a pipeline connection.
   */
  void AddInputData( vtkDataObject* );
  void AddInputData( int, vtkDataObject* );
  //@}

protected:
  vtkHyperTreeGridAlgorithm();
  ~vtkHyperTreeGridAlgorithm() VTK_OVERRIDE;

  // convenience method
  virtual int RequestInformation( vtkInformation*,
                                  vtkInformationVector**,
                                  vtkInformationVector* );

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestData( vtkInformation*,
                           vtkInformationVector**,
                           vtkInformationVector* );

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestUpdateExtent( vtkInformation*,
                                   vtkInformationVector**,
                                   vtkInformationVector* );

  /**
   * Main routine to process individual trees in the grid
   * This is pure virtual method to be implemented by concrete algorithms
   */
  virtual int ProcessTrees( vtkHyperTreeGrid*,
                            vtkDataObject* ) = 0;

  //@{
  /**
   * Define default input and output port types
   */
  int FillInputPortInformation( int, vtkInformation* ) VTK_OVERRIDE;
  int FillOutputPortInformation( int, vtkInformation* ) VTK_OVERRIDE;
  //@}

  vtkDataSetAttributes* InData;
  vtkDataSetAttributes* OutData;

private:
  vtkHyperTreeGridAlgorithm(const vtkHyperTreeGridAlgorithm&) VTK_DELETE_FUNCTION;
  void operator=(const vtkHyperTreeGridAlgorithm&) VTK_DELETE_FUNCTION;
};

#endif