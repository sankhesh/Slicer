vtk_module(vtkIOSegY
  TEST_DEPENDS
    vtkInteractionStyle
    vtkRenderingOpenGL2
    vtkTestingCore
    vtkTestingRendering
  KIT
    vtkIO
  DEPENDS
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkIOCore
  PRIVATE_DEPENDS
    vtkCommonCore
  )