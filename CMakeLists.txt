PROJECT(CreateIntensities)
cmake_minimum_required(VERSION 2.8)
#FIND_PACKAGE ( ITK)
#IF ( ITK_FOUND)
 # INCLUDE( ${USE_ITK_FILE} )
#ENDIF( ITK_FOUND)

FIND_PACKAGE ( VTK)
IF ( VTK_FOUND)
  INCLUDE( ${USE_VTK_FILE} )
ENDIF( VTK_FOUND)

INCLUDE_DIRECTORIES(
${CreateIntensities_SOURCE_DIR}
)
ADD_EXECUTABLE( CreateIntensities CreateMissingIntensities.cxx
				SelectQueryDatabaseHelper.cxx
				QueryDataBaseHelper.cxx
				GoDBIntensityRow.cxx
				GoDBRow.cxx
				)
TARGET_LINK_LIBRARIES ( CreateIntensities
#ITKBasicFilters ITKCommon ITKIO
vtkRendering vtkGraphics vtkHybrid
vtkImaging vtkIO vtkFiltering vtkCommon
)