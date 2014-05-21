

#Find pugixml
FIND_LIBRARY(PUGIXML_LIBRARY 
  NAMES pugixml
  PATHS "/usr/lib"
  "/usr/lib64"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 8/VC/PlatformSDK/Lib"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 9.0/VC/lib/"
  "$ENV{PROGRAMW6432}/Microsoft SDKs/Windows/v6.0A/Lib"
  "~/mylibs/pugixml/lib"
  "$ENV{ProgramFiles}/Microsoft SDKs/Windows/v7.0A/Lib"
  "${PUGIXML_ROOT}/lib"
  )
  
FIND_LIBRARY(PUGIXML_LIBRARY_DEBUG
  NAMES pugixmld
  PATHS "/usr/lib"
  "/usr/lib64"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 8/VC/PlatformSDK/Lib"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 9.0/VC/lib/"
  "$ENV{PROGRAMW6432}/Microsoft SDKs/Windows/v6.0A/Lib"
  "~/mylibs/pugixml/lib"
  "$ENV{ProgramFiles}/Microsoft SDKs/Windows/v7.0A/Lib"
  "${PUGIXML_ROOT}/lib"
  )

#Find pugixml header
FIND_PATH(PUGIXML_INCLUDE_DIR "pugixml.hpp"
  "/usr/include"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 8/VC/PlatformSDK/Include"
  "$ENV{ProgramFiles}/Microsoft Visual Studio 9.0/VC/include/"
  "$ENV{PROGRAMW6432}/Microsoft SDKs/Windows/v6.0A/Include"
  "~/mylibs/pugixml/include"
  "$ENV{ProgramFiles}/Microsoft SDKs/Windows/v7.0A/Include"
  "${PUGIXML_ROOT}/include"
)

#check that we have found everything
SET(PUGIXML_FOUND "NO")
IF(PUGIXML_LIBRARY)
  IF(PUGIXML_LIBRARY_DEBUG) 
	IF(PUGIXML_INCLUDE_DIR)
		SET(PUGIXML_FOUND "YES")
		SET(PUGIXML_LIBRARIES ${PUGIXML_LIBRARY})
		SET(PUGIXML_LIBRARIES_DEBUG ${PUGIXML_LIBRARY_DEBUG})
	ENDIF(PUGIXML_INCLUDE_DIR)
  ENDIF(PUGIXML_LIBRARY_DEBUG)
ENDIF(PUGIXML_LIBRARY)

#Mark options as advanced
MARK_AS_ADVANCED(
  PUGIXML_INCLUDE_DIR
  PUGIXML_LIBRARY
  PUGIXML_LIBRARIES
  PUGIXML_LIBRARIES_DEBUG
)