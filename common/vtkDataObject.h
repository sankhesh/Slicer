/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDataObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-1998 Ken Martin, Will Schroeder, Bill Lorensen.

This software is copyrighted by Ken Martin, Will Schroeder and Bill Lorensen.
The following terms apply to all files associated with the software unless
explicitly disclaimed in individual files. This copyright specifically does
not apply to the related textbook "The Visualization Toolkit" ISBN
013199837-4 published by Prentice Hall which is covered by its own copyright.

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


=========================================================================*/
// .NAME vtkDataObject - general representation of visualization data
// .SECTION Description
// vtkDataObject is an general representation of visualization data. It serves
// to encapsulate instance variables and methods for visualization network 
// execution, as well as representing data consisting of a field (i.e., just
// an unstructured pile of data). This is to be compared with a vtkDataSet,
// which is data with geometric and/or topological structure.
//
// vtkDataObjects are used to represent arbitrary repositories of data via the
// vtkFieldData instance variable. These data must be eventually mapped into a
// concrete subclass of vtkDataSet before they can actually be displayed.
//
// .SECTION See Also
// vtkDataSet vtkFieldData vtkDataObjectSource vtkDataObjectFilter
// vtkDataObjectMapper vtkDataObjectToDataSet 
// vtkFieldDataToAttributeDataFilter

#ifndef __vtkDataObject_h
#define __vtkDataObject_h

#include "vtkObject.h"
#include "vtkFieldData.h"

class vtkSource;
class vtkExtent;
class vtkDataInformation;

class VTK_EXPORT vtkDataObject : public vtkObject
{
public:
  static vtkDataObject *New() {return new vtkDataObject;};

  const char *GetClassName() {return "vtkDataObject";};
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Create concrete instance of this data object.
  virtual vtkDataObject *MakeObject() {return new vtkDataObject;}
  

  // Description:
  // Set/Get the source object creating this data object.
  vtkGetObjectMacro(Source,vtkSource);
  void SetSource(vtkSource *s);
  
  // Description:
  // Data objects are composite objects and need to check each part for MTime.
  // The information object also needs to be considered.
  unsigned long int GetMTime();

  // Rescription:
  // Restore data object to initial state,
  virtual void Initialize();

  // Description:
  // Release data back to system to conserve memory resource. Used during
  // visualization network execution.  Releasing this data does not make 
  // down-stream data invalid, so it does not modify the MTime of this 
  // data object.
  void ReleaseData();

  // Description:
  // Return flag indicating whether data should be released after use  
  // by a filter.
  int ShouldIReleaseData();

  // Description:
  // Get the flag indicating the data has been released.
  vtkGetMacro(DataReleased,int);
  
  // Description:
  // Turn on/off flag to control whether this object's data is released
  // after being used by a filter.
  vtkSetMacro(ReleaseDataFlag,int);
  vtkGetMacro(ReleaseDataFlag,int);
  vtkBooleanMacro(ReleaseDataFlag,int);

  // Description:
  // Turn on/off flag to control whether every object releases its data
  // after being used by a filter.
  static void SetGlobalReleaseDataFlag(int val);
  void GlobalReleaseDataFlagOn() {this->SetGlobalReleaseDataFlag(1);};
  void GlobalReleaseDataFlagOff() {this->SetGlobalReleaseDataFlag(0);};
  static int GetGlobalReleaseDataFlag();

  // Description:
  // Assign or retrieve field data to this data object.
  vtkSetObjectMacro(FieldData,vtkFieldData);
  vtkGetObjectMacro(FieldData,vtkFieldData);
  
  // Handle the source/data loop.
  void UnRegister(vtkObject *o);

  // Description:
  // Get the net reference count. That is the count minus
  // any self created loops. This is used in the Source/Data
  // registration to properly free the objects.
  virtual int GetNetReferenceCount() {return this->ReferenceCount;};


  //------------- streaming stuff ------------------

  // Description:
  // Provides opportunity for the data object to insure internal 
  // consistency before access. Also causes owning source/filter 
  // (if any) to update itself.
  virtual void Update();

  // Description:
  // This is the same as Update, but it assumes that the "Information" 
  // (including the PipelineMTime) is up to date.  
  virtual void InternalUpdate();

  // Description:
  // This makes sure all "Information" associated with this data object
  // is up to date.  Information is defined as any thing that is needed
  // before the input is updated (like PipelineMTime for the execution check
  // and EstimatedWholeMemorySize for streaming).
  virtual void UpdateInformation();
  
  // Description:
  // A generic way of specifying an update extent.  Subclasses
  // must decide what a piece is.
  virtual void SetUpdateExtent(int vtkNotUsed(piece),int vtkNotUsed(numPieces))
    { vtkErrorMacro("Subclass did not implement 'SetUpdateExtent'");}
  
  // Description:
  // Set/Get memory limit.  Make this smaller to stream.
  // Setting value does not alter MTime.
  void SetMemoryLimit(unsigned long v) {this->MemoryLimit = v;}
  unsigned long GetMemoryLimit() {return this->MemoryLimit;}

  // Description:
  // One of the variables set when UpdateInformation is called.
  // It is the estimated size of the data (in kilobytes) after 
  // the whole extent is updated.
  void SetEstimatedWholeMemorySize(unsigned long v);
  virtual unsigned long GetEstimatedWholeMemorySize();
  
  // Description:
  // Convience method: Uses the EstimatedWholeMemorySize to compute
  // the estimated memory size of the update extent.
  virtual unsigned long GetEstimatedUpdateMemorySize()
    { vtkErrorMacro(
      "Subclass did not implement 'GetEstimatedUpdateMemorySize'");
    return 0;}
  
  // Description:
  // Return the actual size of the data in kilobytes. This number
  // is valid only after the pipeline has updated. The memory size
  // returned is guaranteed to be greater than or equal to the
  // memory required to represent the data (e.g., extra space in
  // arrays, etc. are not included in the return value).
  virtual unsigned long GetActualMemorySize();
  
  // Description:
  // PipelineMTime is the maximum of all the upstream source object mtimes.
  // It does not include mtimes of the data objects.
  // UpdateInformation must be called for the PipelineMTime to be correct.
  // Only the source should set the PipelineMTime.
  void SetPipelineMTime(long t);
  long GetPipelineMTime();
  
  // Description:
  // Copies the UpdateExtent from another dataset of the same type.
  // Used by a filter during UpdateInformation to copy requested 
  // piece from output to input.  
  void CopyUpdateExtent(vtkDataObject *data); 

  // Description:
  // Warning: This is still in develoment.  DataSetToDataSetFilters use
  // CopyUpdateExtent to pass the update extents up the pipeline.
  // In order to pass a generic update extent through a port we are going 
  // to need these methods (which should eventually replace the 
  // CopyUpdateExtent method).
  virtual vtkExtent *GetGenericUpdateExtent() { return this->UpdateExtent; }
  virtual void CopyGenericUpdateExtent(vtkExtent *vtkNotUsed(ext))
    {vtkErrorMacro("Subclass did not implent CopyGenericUpdateExtent");}  
  
  // Description:
  // Warning: This is still in develoment.  
  // Eventually we should be able to eliminate the CopyInformationMethod
  vtkDataInformation *GetDataInformation() {return this->Information;}
  
  // Description:
  // Copies "Information" (ie WholeDimensions) from another dataset 
  // of the same type.  Used by a filter during UpdateInformation. 
  void CopyInformation(vtkDataObject *data); 

  // Description:
  // Return class name of data type. This is one of VTK_STRUCTURED_GRID, 
  // VTK_STRUCTURED_POINTS, VTK_UNSTRUCTURED_GRID, VTK_POLY_DATA, or
  // VTK_RECTILINEAR_GRID (see vtkSetGet.h for definitions).
  // THIS METHOD IS THREAD SAFE
  virtual int GetDataObjectType() {return VTK_DATA_OBJECT;}
  
  // Description:
  // Used by Threaded ports to determine if they should initiate an
  // asynchronous update (still in development).
  unsigned long GetUpdateTime();

  // Dscription:
  // A non-blocking InternalUpdate for ports. This method has a side effect
  // of propagating the UpdateExtents up the pipeline.
  void PreUpdate();
  
protected:

  vtkDataObject();
  ~vtkDataObject();
  vtkDataObject(const vtkDataObject&) {};
  void operator=(const vtkDataObject&) {};

  // Description:
  // Method implemented in the subclasses to make sure the update extent
  // is not bigger than the whole extent.  Should be pure virtual.
  // If the UpdateExtent does not overlap with the WholeExtent, then
  // the method returns 0, otherwise it returns 1.
  // It also has the task of releasing the current data if it will not
  // satisfy the UpdateExtent request.  This should really be somewhere
  // else...
  virtual int ClipUpdateExtentWithWholeExtent() {return 1;}  
  
  
  vtkFieldData *FieldData; //General field data associated with data object  
  vtkSource *Source;

  int DataReleased; //keep track of data release during network execution
  int ReleaseDataFlag; //data will release after use by a filter
  vtkTimeStamp UpdateTime;

  // Between PreUpdate and InternalUpdate we are "WaitingForUpdate".
  // This enforces the one to one corespondence between PreUpdate
  // and InternalUpdate.
  int WaitingForUpdate;
  
  // The input of a filter holds the memory limit that triggers streamining.
  // Not all filters will respect this limit.
  unsigned long MemoryLimit;

  // All the information is containted in this object now.
  vtkDataInformation *Information;
  
  // The UpdateExtent is stored here.
  vtkExtent *UpdateExtent;
};

#endif

