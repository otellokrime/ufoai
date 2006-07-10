; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "UFO:Alien Invasion"
!define PRODUCT_VERSION "2.0-RC2"
!define PRODUCT_PUBLISHER "UFO:AI Team"
!define PRODUCT_WEB_SITE "http://www.ufoai.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\ufo.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor bzip2

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "..\..\COPYING"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
;!define MUI_FINISHPAGE_RUN "$INSTDIR\ufo.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "ufoai-${PRODUCT_VERSION}-win32.exe"
InstallDir "$PROGRAMFILES\UFOAI"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Game" SEC01
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR"
  File "..\..\contrib\*.dll"
  File "..\..\*.dll"
  File "..\..\*.exe"
  File "..\..\.gamedir"
  SetOutPath "$INSTDIR\base"
  File "..\..\base\*.cfg"
  File "..\..\base\*.dll"
  SetOutPath "$INSTDIR\base\i18n"
  SetOutPath "$INSTDIR\base\i18n\en_GB"
  SetOutPath "$INSTDIR\base\i18n\en_GB\LC_MESSAGES"
  File "..\..\base\i18n\en_GB\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\de\LC_MESSAGES"
  File "..\..\base\i18n\de\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\ru\LC_MESSAGES"
  File "..\..\base\i18n\ru\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\cs\LC_MESSAGES"
  File "..\..\base\i18n\cs\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\da\LC_MESSAGES"
  File "..\..\base\i18n\da\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\est\LC_MESSAGES"
  File "..\..\base\i18n\est\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\fr\LC_MESSAGES"
  File "..\..\base\i18n\fr\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\it\LC_MESSAGES"
  File "..\..\base\i18n\it\LC_MESSAGES\*.mo"
  SetOutPath "$INSTDIR\base\i18n\pl\LC_MESSAGES"
  File "..\..\base\i18n\pl\LC_MESSAGES\*.mo"

; models
  SetOutPath "$INSTDIR\base\models"

;======================================================================
; models/aircraft
;======================================================================
  SetOutPath "$INSTDIR\base\models\aircraft"
  SetOutPath "$INSTDIR\base\models\aircraft\dropship"
  File "..\..\base\models\aircraft\dropship\*.md2"
  File "..\..\base\models\aircraft\dropship\*.jpg"
  SetOutPath "$INSTDIR\base\models\aircraft\interceptor"
  File "..\..\base\models\aircraft\interceptor\*.md2"
  File "..\..\base\models\aircraft\interceptor\*.jpg"
  SetOutPath "$INSTDIR\base\models\aircraft\ufo_big"
  File "..\..\base\models\aircraft\ufo_big\*.md2"
  File "..\..\base\models\aircraft\ufo_big\*.jpg"
  SetOutPath "$INSTDIR\base\models\aircraft\ufo_small"
  File "..\..\base\models\aircraft\ufo_small\*.md2"
  File "..\..\base\models\aircraft\ufo_small\*.jpg"

;======================================================================
; models/aliens
;======================================================================
; FIXME: Activate md2 and other
;======================================================================
  SetOutPath "$INSTDIR\base\models\aliens"
  SetOutPath "$INSTDIR\base\models\aliens\blue"
  File "..\..\base\models\aliens\blue\*.md2"
  File "..\..\base\models\aliens\blue\*.jpg"
  File "..\..\base\models\aliens\blue\*.tag"
  File "..\..\base\models\aliens\blue\*.anm"
  SetOutPath "$INSTDIR\base\models\aliens\ortnok"
  File "..\..\base\models\aliens\ortnok\*.md2"
  File "..\..\base\models\aliens\ortnok\*.jpg"
  File "..\..\base\models\aliens\ortnok\*.tag"
  File "..\..\base\models\aliens\ortnok\*.anm"
  SetOutPath "$INSTDIR\base\models\aliens\ortnok_light"
  File "..\..\base\models\aliens\ortnok_light\*.md2"
  File "..\..\base\models\aliens\ortnok_light\*.jpg"
  File "..\..\base\models\aliens\ortnok_light\*.tag"
  File "..\..\base\models\aliens\ortnok_light\*.anm"
  SetOutPath "$INSTDIR\base\models\aliens\ortnok_medium"
  File "..\..\base\models\aliens\ortnok_medium\*.md2"
  File "..\..\base\models\aliens\ortnok_medium\*.jpg"
  File "..\..\base\models\aliens\ortnok_medium\*.tag"
  File "..\..\base\models\aliens\ortnok_medium\*.anm"
  SetOutPath "$INSTDIR\base\models\aliens\hovernet"
  ;File "..\..\base\models\aliens\hovernet\*.md2"
  File "..\..\base\models\aliens\hovernet\*.jpg"
  ;File "..\..\base\models\aliens\hovernet\*.tag"
  ;File "..\..\base\models\aliens\hovernet\*.anm"

;======================================================================
; models/animals
;======================================================================
  SetOutPath "$INSTDIR\base\models\animals"
  SetOutPath "$INSTDIR\base\models\animals\cow"
  File "..\..\base\models\animals\cow\*.anm"
  File "..\..\base\models\animals\cow\*.tag"
  File "..\..\base\models\animals\cow\*.jpg"
  File "..\..\base\models\animals\cow\*.md2"
  SetOutPath "$INSTDIR\base\models\animals\penguin"
  File "..\..\base\models\animals\penguin\*.anm"
  File "..\..\base\models\animals\penguin\*.tag"
  File "..\..\base\models\animals\penguin\*.jpg"
  File "..\..\base\models\animals\penguin\*.md2"

;======================================================================
; models/civilians
;======================================================================
  SetOutPath "$INSTDIR\base\models\civilians"
  SetOutPath "$INSTDIR\base\models\civilians\female"
  File "..\..\base\models\civilians\female\*.anm"
  File "..\..\base\models\civilians\female\*.jpg"
  File "..\..\base\models\civilians\female\*.md2"
  File "..\..\base\models\civilians\female\*.tag"
  SetOutPath "$INSTDIR\base\models\civilians\male"
  File "..\..\base\models\civilians\male\*.anm"
  File "..\..\base\models\civilians\male\*.jpg"
  File "..\..\base\models\civilians\male\*.md2"
  File "..\..\base\models\civilians\male\*.tag"

;======================================================================
; models/objects
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects"

;======================================================================
; models/objects/alien
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\alien"
  SetOutPath "$INSTDIR\base\models\objects\alien\battery"
  File "..\..\base\models\objects\alien\battery\*.jpg"
  File "..\..\base\models\objects\alien\battery\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\alien\ion_condenser"
  File "..\..\base\models\objects\alien\ion_condenser\*.jpg"
  File "..\..\base\models\objects\alien\ion_condenser\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\alien\panel"
  File "..\..\base\models\objects\alien\panel\*.jpg"
  File "..\..\base\models\objects\alien\panel\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\alien\pstation"
  File "..\..\base\models\objects\alien\pstation\*.jpg"
  File "..\..\base\models\objects\alien\pstation\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\alien\seat"
  File "..\..\base\models\objects\alien\seat\*.jpg"
  File "..\..\base\models\objects\alien\seat\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\alien\ufo_crashed"
  File "..\..\base\models\objects\alien\ufo_crashed\*.jpg"
  File "..\..\base\models\objects\alien\ufo_crashed\*.md2"

;======================================================================
; models/objects/base
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\base"

;======================================================================
; models/objects/base/radar
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\base\radar"
  File "..\..\base\models\objects\base\radar\*.jpg"
  File "..\..\base\models\objects\base\radar\*.md2"

;======================================================================
; models/objects/cars
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\cars"
  SetOutPath "$INSTDIR\base\models\objects\cars\car2"
  File "..\..\base\models\objects\cars\car2\*.jpg"
  File "..\..\base\models\objects\cars\car2\*.md2"
  File "..\..\base\models\objects\cars\car2\*.tga"
  SetOutPath "$INSTDIR\base\models\objects\cars\golf"
  File "..\..\base\models\objects\cars\golf\*.jpg"
  File "..\..\base\models\objects\cars\golf\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\cars\van"
  File "..\..\base\models\objects\cars\van\*.jpg"
  File "..\..\base\models\objects\cars\van\*.md2"

;======================================================================
; models/objects/city
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\city"
  File "..\..\base\models\objects\city\*.jpg"
  File "..\..\base\models\objects\city\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\city\light"
  File "..\..\base\models\objects\city\light\*.jpg"
  File "..\..\base\models\objects\city\light\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\city\signs"
  File "..\..\base\models\objects\city\signs\*.jpg"
  File "..\..\base\models\objects\city\signs\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\city\trash"
  File "..\..\base\models\objects\city\trash\*.jpg"
  File "..\..\base\models\objects\city\trash\*.md2"

;======================================================================
; models/objects/furniture
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\furniture"
  SetOutPath "$INSTDIR\base\models\objects\furniture\bed"
  File "..\..\base\models\objects\furniture\bed\*.jpg"
  File "..\..\base\models\objects\furniture\bed\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\bookshelf"
  File "..\..\base\models\objects\furniture\bookshelf\*.jpg"
  File "..\..\base\models\objects\furniture\bookshelf\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\computer"
  File "..\..\base\models\objects\furniture\computer\*.jpg"
  File "..\..\base\models\objects\furniture\computer\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\cupboard"
  File "..\..\base\models\objects\furniture\cupboard\*.jpg"
  File "..\..\base\models\objects\furniture\cupboard\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\flowerpot"
  File "..\..\base\models\objects\furniture\flowerpot\*.tga"
  File "..\..\base\models\objects\furniture\flowerpot\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\laptop"
  File "..\..\base\models\objects\furniture\laptop\*.jpg"
  File "..\..\base\models\objects\furniture\laptop\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\locker"
  File "..\..\base\models\objects\furniture\locker\*.jpg"
  File "..\..\base\models\objects\furniture\locker\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\metalshelf"
  File "..\..\base\models\objects\furniture\metalshelf\*.jpg"
  File "..\..\base\models\objects\furniture\metalshelf\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\speaker"
  File "..\..\base\models\objects\furniture\speaker\*.jpg"
  File "..\..\base\models\objects\furniture\speaker\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\sunprot"
  File "..\..\base\models\objects\furniture\sunprot\*.jpg"
  File "..\..\base\models\objects\furniture\sunprot\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\furniture\trashcan"
  File "..\..\base\models\objects\furniture\trashcan\*.jpg"
  File "..\..\base\models\objects\furniture\trashcan\*.md2"

;======================================================================
; models/objects/vegi
;======================================================================
  SetOutPath "$INSTDIR\base\models\objects\vegi"
  SetOutPath "$INSTDIR\base\models\objects\vegi\bush"
  File "..\..\base\models\objects\vegi\bush\*.jpg"
  File "..\..\base\models\objects\vegi\bush\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\cactus"
  File "..\..\base\models\objects\vegi\cactus\*.jpg"
  File "..\..\base\models\objects\vegi\cactus\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\chestnut"
  File "..\..\base\models\objects\vegi\chestnut\*.jpg"
  File "..\..\base\models\objects\vegi\chestnut\*.md2"
  File "..\..\base\models\objects\vegi\chestnut\*.tga"
  SetOutPath "$INSTDIR\base\models\objects\vegi\corn"
  File "..\..\base\models\objects\vegi\corn\*.tga"
  File "..\..\base\models\objects\vegi\corn\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm1"
  File "..\..\base\models\objects\vegi\palm1\*.jpg"
  File "..\..\base\models\objects\vegi\palm1\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm2"
  File "..\..\base\models\objects\vegi\palm2\*.tga"
  File "..\..\base\models\objects\vegi\palm2\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm3"
  File "..\..\base\models\objects\vegi\palm3\*.tga"
  File "..\..\base\models\objects\vegi\palm3\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm4"
  File "..\..\base\models\objects\vegi\palm4\*.jpg"
  File "..\..\base\models\objects\vegi\palm4\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm5"
  File "..\..\base\models\objects\vegi\palm5\*.jpg"
  File "..\..\base\models\objects\vegi\palm5\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm6"
  File "..\..\base\models\objects\vegi\palm6\*.jpg"
  File "..\..\base\models\objects\vegi\palm6\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\palm7"
  File "..\..\base\models\objects\vegi\palm7\*.jpg"
  File "..\..\base\models\objects\vegi\palm7\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\tree01"
  File "..\..\base\models\objects\vegi\tree01\*.tga"
  File "..\..\base\models\objects\vegi\tree01\*.jpg"
  File "..\..\base\models\objects\vegi\tree01\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\tree02"
  File "..\..\base\models\objects\vegi\tree02\*.tga"
  File "..\..\base\models\objects\vegi\tree02\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\tropic"
  File "..\..\base\models\objects\vegi\tropic\*.tga"
  File "..\..\base\models\objects\vegi\tropic\*.jpg"
  File "..\..\base\models\objects\vegi\tropic\*.md2"
  SetOutPath "$INSTDIR\base\models\objects\vegi\tropical1"
  File "..\..\base\models\objects\vegi\tropical1\*.tga"
  File "..\..\base\models\objects\vegi\tropical1\*.md2"

;======================================================================
; models/soldiers
;======================================================================
  SetOutPath "$INSTDIR\base\models\soldiers"
  SetOutPath "$INSTDIR\base\models\soldiers\female"
  File "..\..\base\models\soldiers\female\*.anm"
  File "..\..\base\models\soldiers\female\*.md2"
  File "..\..\base\models\soldiers\female\*.tag"
  File "..\..\base\models\soldiers\female\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\femalelight"
  File "..\..\base\models\soldiers\femalelight\*.anm"
  File "..\..\base\models\soldiers\femalelight\*.md2"
  File "..\..\base\models\soldiers\femalelight\*.tag"
  File "..\..\base\models\soldiers\femalelight\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\femalemedium"
  File "..\..\base\models\soldiers\femalemedium\*.anm"
  File "..\..\base\models\soldiers\femalemedium\*.md2"
  File "..\..\base\models\soldiers\femalemedium\*.tag"
  File "..\..\base\models\soldiers\femalemedium\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\male"
  File "..\..\base\models\soldiers\male\*.anm"
  File "..\..\base\models\soldiers\male\*.md2"
  File "..\..\base\models\soldiers\male\*.tag"
  File "..\..\base\models\soldiers\male\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\malelight"
  File "..\..\base\models\soldiers\malelight\*.anm"
  File "..\..\base\models\soldiers\malelight\*.md2"
  File "..\..\base\models\soldiers\malelight\*.tag"
  File "..\..\base\models\soldiers\malelight\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\malemedium"
  File "..\..\base\models\soldiers\malemedium\*.anm"
  File "..\..\base\models\soldiers\malemedium\*.md2"
  File "..\..\base\models\soldiers\malemedium\*.tag"
  File "..\..\base\models\soldiers\malemedium\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\ugv_phoenix"
  File "..\..\base\models\soldiers\ugv_phoenix\*.md2"
  File "..\..\base\models\soldiers\ugv_phoenix\*.jpg"
  SetOutPath "$INSTDIR\base\models\soldiers\ugv_triax"
  File "..\..\base\models\soldiers\ugv_triax\*.md2"
  File "..\..\base\models\soldiers\ugv_triax\*.jpg"

;======================================================================
; models/weapons
;======================================================================
  SetOutPath "$INSTDIR\base\models\weapons"
  SetOutPath "$INSTDIR\base\models\weapons\assault"
  File "..\..\base\models\weapons\assault\*.jpg"
  File "..\..\base\models\weapons\assault\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\bolterrifle"
  File "..\..\base\models\weapons\bolterrifle\*.jpg"
  File "..\..\base\models\weapons\bolterrifle\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\flamer"
  File "..\..\base\models\weapons\flamer\*.jpg"
  File "..\..\base\models\weapons\flamer\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\flare"
  File "..\..\base\models\weapons\flare\*.tga"
  File "..\..\base\models\weapons\flare\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\fraggren"
  File "..\..\base\models\weapons\fraggren\*.jpg"
  File "..\..\base\models\weapons\fraggren\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\grenl"
  File "..\..\base\models\weapons\grenl\*.jpg"
  File "..\..\base\models\weapons\grenl\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\incgren"
  File "..\..\base\models\weapons\incgren\*.tga"
  File "..\..\base\models\weapons\incgren\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\irgoggles"
  File "..\..\base\models\weapons\irgoggles\*.jpg"
  File "..\..\base\models\weapons\irgoggles\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\kerrblade"
  File "..\..\base\models\weapons\kerrblade\*.jpg"
  File "..\..\base\models\weapons\kerrblade\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\knife"
  File "..\..\base\models\weapons\knife\*.jpg"
  File "..\..\base\models\weapons\knife\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\laserpistol"
  File "..\..\base\models\weapons\laserpistol\*.jpg"
  File "..\..\base\models\weapons\laserpistol\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\laserrifle"
  File "..\..\base\models\weapons\laserrifle\*.jpg"
  File "..\..\base\models\weapons\laserrifle\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\medikit"
  File "..\..\base\models\weapons\medikit\*.jpg"
  File "..\..\base\models\weapons\medikit\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\pistol"
  File "..\..\base\models\weapons\pistol\*.jpg"
  File "..\..\base\models\weapons\pistol\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\plasblaster"
  File "..\..\base\models\weapons\plasblaster\*.jpg"
  File "..\..\base\models\weapons\plasblaster\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\plaspistol"
  File "..\..\base\models\weapons\plaspistol\*.jpg"
  File "..\..\base\models\weapons\plaspistol\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\plasrifle"
  File "..\..\base\models\weapons\plasrifle\*.jpg"
  File "..\..\base\models\weapons\plasrifle\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\rpg"
  File "..\..\base\models\weapons\rpg\*.jpg"
  File "..\..\base\models\weapons\rpg\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\shotgun"
  File "..\..\base\models\weapons\shotgun\*.jpg"
  File "..\..\base\models\weapons\shotgun\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\smg"
  File "..\..\base\models\weapons\smg\*.jpg"
  File "..\..\base\models\weapons\smg\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\sniper"
  File "..\..\base\models\weapons\sniper\*.jpg"
  File "..\..\base\models\weapons\sniper\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\stungren"
  File "..\..\base\models\weapons\stungren\*.tga"
  File "..\..\base\models\weapons\stungren\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\stunrod"
  File "..\..\base\models\weapons\stunrod\*.jpg"
  File "..\..\base\models\weapons\stunrod\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\tachyonpistol"
  File "..\..\base\models\weapons\tachyonpistol\*.jpg"
  File "..\..\base\models\weapons\tachyonpistol\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\tachyonrifle"
  File "..\..\base\models\weapons\tachyonrifle\*.jpg"
  File "..\..\base\models\weapons\tachyonrifle\*.md2"
  SetOutPath "$INSTDIR\base\models\weapons\tachyonsniper"
  File "..\..\base\models\weapons\tachyonsniper\*.jpg"
  File "..\..\base\models\weapons\tachyonsniper\*.md2"

;======================================================================
; music
;======================================================================
  SetOutPath "$INSTDIR\base\music"
  File "..\..\base\music\*.txt"
  File /nonfatal "..\..\base\music\*.ogg"

;======================================================================
; media
;======================================================================
  SetOutPath "$INSTDIR\base\media"
  File "..\..\base\media\COPYING"
  File "..\..\base\media\*.ttf"

;======================================================================
; pics
;======================================================================
  SetOutPath "$INSTDIR\base\pics"
  File "..\..\base\pics\*.jpg"
  File "..\..\base\pics\*.tga"
  File "..\..\base\pics\*.pcx"
  SetOutPath "$INSTDIR\base\pics\althud"
  File "..\..\base\pics\althud\*.tga"
  SetOutPath "$INSTDIR\base\pics\armor"
  File "..\..\base\pics\armor\*.tga"
  SetOutPath "$INSTDIR\base\pics\base"
  File "..\..\base\pics\base\*.tga"
  SetOutPath "$INSTDIR\base\pics\hud"
  File "..\..\base\pics\hud\*.tga"
  SetOutPath "$INSTDIR\base\pics\maps"
  SetOutPath "$INSTDIR\base\pics\maps\shots"
  File "..\..\base\pics\maps\shots\*.jpg"
  SetOutPath "$INSTDIR\base\pics\menu"
  File "..\..\base\pics\menu\*.tga"
  File "..\..\base\pics\menu\*.jpg"
  SetOutPath "$INSTDIR\base\pics\sfx"
  File "..\..\base\pics\sfx\*.tga"
  File "..\..\base\pics\sfx\*.jpg"

;======================================================================
; save
;======================================================================
  SetOutPath "$INSTDIR\base\save"

;======================================================================
; sounds
;======================================================================
  SetOutPath "$INSTDIR\base\sound"
  SetOutPath "$INSTDIR\base\sound\misc"
  File "..\..\base\sound\misc\*.wav"
  SetOutPath "$INSTDIR\base\sound\weapons"
  File "..\..\base\sound\weapons\*.wav"

;======================================================================
; textures
;======================================================================
  SetOutPath "$INSTDIR\base\textures"
  SetOutPath "$INSTDIR\base\textures\exterior"
  File "..\..\base\textures\exterior\*.jpg"
  SetOutPath "$INSTDIR\base\textures\interior"
  File "..\..\base\textures\interior\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_base"
  File "..\..\base\textures\tex_base\*.jpg"
  File "..\..\base\textures\tex_base\*.tga"
  SetOutPath "$INSTDIR\base\textures\tex_buildings"
  File "..\..\base\textures\tex_buildings\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_buildings_snow"
  File "..\..\base\textures\tex_buildings_snow\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_doors"
  File "..\..\base\textures\tex_doors\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_effects"
  File "..\..\base\textures\tex_effects\*.tga"
  SetOutPath "$INSTDIR\base\textures\tex_furniture"
  File "..\..\base\textures\tex_furniture\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_lights"
  File "..\..\base\textures\tex_lights\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_material"
  File "..\..\base\textures\tex_material\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_misc"
  File "..\..\base\textures\tex_misc\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_nature"
  File "..\..\base\textures\tex_nature\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_pics"
  File "..\..\base\textures\tex_pics\*.jpg"
  File "..\..\base\textures\tex_pics\*.tga"
  SetOutPath "$INSTDIR\base\textures\tex_shop"
  File "..\..\base\textures\tex_shop\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_streets"
  File "..\..\base\textures\tex_streets\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_tech"
  File "..\..\base\textures\tex_tech\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_ufo"
  File "..\..\base\textures\tex_ufo\*.jpg"
  SetOutPath "$INSTDIR\base\textures\tex_vehicles"
  File "..\..\base\textures\tex_vehicles\*.jpg"

;======================================================================
; ufos
;======================================================================
  SetOutPath "$INSTDIR\base\ufos"
  File "..\..\base\ufos\*.ufo"

;======================================================================
; maps
;======================================================================
  SetOutPath "$INSTDIR\base\maps"
  File "..\..\base\maps\*.bsp"
  File "..\..\base\maps\*.ump"
  SetOutPath "$INSTDIR\base\maps\b"
  SetOutPath "$INSTDIR\base\maps\b\a"
  File "..\..\base\maps\b\a\*.bsp"
  SetOutPath "$INSTDIR\base\maps\b\d"
  File "..\..\base\maps\b\d\*.bsp"
  SetOutPath "$INSTDIR\base\maps\b\g"
  File "..\..\base\maps\b\g\*.bsp"
  SetOutPath "$INSTDIR\base\maps\frozend"
  File "..\..\base\maps\frozend\*.bsp"
  SetOutPath "$INSTDIR\base\maps\frozenn"
  File "..\..\base\maps\frozenn\*.bsp"
  SetOutPath "$INSTDIR\base\maps\ice"
  File "..\..\base\maps\ice\*.bsp"
  SetOutPath "$INSTDIR\base\maps\orientald"
  File "..\..\base\maps\orientald\*.bsp"
  SetOutPath "$INSTDIR\base\maps\orientaln"
  File "..\..\base\maps\orientaln\*.bsp"
  SetOutPath "$INSTDIR\base\maps\tropicd"
  File "..\..\base\maps\tropicd\*.bsp"
  SetOutPath "$INSTDIR\base\maps\tropicn"
  File "..\..\base\maps\tropicn\*.bsp"
  SetOutPath "$INSTDIR\base\maps\villaged"
  File "..\..\base\maps\villaged\*.bsp"
  SetOutPath "$INSTDIR\base\maps\villagen"
  File "..\..\base\maps\villagen\*.bsp"

;======================================================================
; to let the game start up
;======================================================================
  SetOutPath "$INSTDIR"

  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}\"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\ufo.exe" "" "$INSTDIR\ufo.exe" 0
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\ufo.exe"
SectionEnd

Section "MappingTools" SEC02
  SetOutPath "$INSTDIR\base\maps"
  File "..\..\base\maps\*.map"
  File "..\..\base\maps\Makefile"
  File "..\..\base\maps\Makefile.win"
  File "..\..\base\maps\compile.pl"
  SetOutPath "$INSTDIR\base\maps\b"
  SetOutPath "$INSTDIR\base\maps\b\a"
  File "..\..\base\maps\b\a\*.map"
  SetOutPath "$INSTDIR\base\maps\b\d"
  File "..\..\base\maps\b\d\*.map"
  SetOutPath "$INSTDIR\base\maps\b\g"
  File "..\..\base\maps\b\g\*.map"
  SetOutPath "$INSTDIR\base\maps\frozend"
  File "..\..\base\maps\frozend\*.map"
  SetOutPath "$INSTDIR\base\maps\frozenn"
  File "..\..\base\maps\frozenn\*.map"
  SetOutPath "$INSTDIR\base\maps\ice"
  File "..\..\base\maps\ice\*.map"
  SetOutPath "$INSTDIR\base\maps\orientald"
  File "..\..\base\maps\orientald\*.map"
  SetOutPath "$INSTDIR\base\maps\orientaln"
  File "..\..\base\maps\orientaln\*.map"
  SetOutPath "$INSTDIR\base\maps\tropicd"
  File "..\..\base\maps\tropicd\*.map"
  SetOutPath "$INSTDIR\base\maps\tropicn"
  File "..\..\base\maps\tropicn\*.map"
  SetOutPath "$INSTDIR\base\maps\villaged"
  File "..\..\base\maps\villaged\*.map"
  SetOutPath "$INSTDIR\base\maps\villagen"
  File "..\..\base\maps\villagen\*.map"
  SetOutPath "$INSTDIR\tools"
  File "..\tools\*.ms"
  File "..\tools\*.qe4"
  File "..\tools\*.def"
  ; EULA
  File "..\..\contrib\*.doc"
  File "..\..\contrib\*.exe"
  File "..\tools\ufo2map\*.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\MAP-Editor.lnk" "$INSTDIR\tools\q3radiant.exe" "" "$INSTDIR\tools\q3radiant.exe" 0
SectionEnd

Section "SourceCode" SEC03
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR\src\client"
  File "..\client\*.h"
  File "..\client\*.c"
  SetOutPath "$INSTDIR\src\docs"
  File "..\docs\*.txt"
  File "..\docs\readme.linux"
  File "..\docs\readme.solaris"
  File "..\docs\TODO"
  SetOutPath "$INSTDIR\src\game"
  File "..\game\*.def"
  File "..\game\*.dev"
  File "..\game\*.dsp"
  File "..\game\*.dsw"
  File "..\game\*.plg"
  File "..\game\*.vcproj"
  File "..\game\*.c"
  File "..\game\*.h"
  SetOutPath "$INSTDIR\src\irix"
  File "..\irix\*.c"
  File "..\irix\Makefile.txt"
  SetOutPath "$INSTDIR\src\unix"
  File "..\unix\*.h"
  File "..\unix\*.c"
  SetOutPath "$INSTDIR\src\linux"
  File "..\linux\*.h"
  File "..\linux\*.c"
  File "..\linux\*.s"
  File "..\linux\*.xbm"
  SetOutPath "$INSTDIR\src"
  File "..\makefile.mingw"
  File "..\Makefile"
  SetOutPath "$INSTDIR\src\null"
  File "..\null\*.c"
  SetOutPath "$INSTDIR\src\po"
  File "..\po\*.po"
  File "..\po\*.pot"
  File "..\po\FINDUFOS"
  File "..\po\Makefile"
  File "..\po\Makevars"
  File "..\po\POTFILES.in"
  File "..\po\ufopo.pl"
  File "..\po\remove-potcdate.sin"
  SetOutPath "$INSTDIR\src\qcommon"
  File "..\qcommon\*.c"
  File "..\qcommon\*.h"
  SetOutPath "$INSTDIR\src\ref_gl"
  File "..\ref_gl\*.h"
  File "..\ref_gl\*.c"
  File "..\ref_gl\*.def"
  File "..\ref_gl\*.dev"
  File "..\ref_gl\*.dsp"
  File "..\ref_gl\*.dsw"
  File "..\ref_gl\*.ncb"
  File "..\ref_gl\*.opt"
  File "..\ref_gl\*.plg"
  File "..\ref_gl\*.vcproj"
  SetOutPath "$INSTDIR\src\server"
  File "..\server\*.h"
  File "..\server\*.c"
  SetOutPath "$INSTDIR\src\solaris"
  File "..\solaris\*.c"
  File "..\solaris\*.h"
  File "..\solaris\Makefile.Solaris"

;======================================================================
; tools
;======================================================================
  SetOutPath "$INSTDIR\src\tools"
  File "..\tools\*.pl"
  File "..\tools\*.ms"
  SetOutPath "$INSTDIR\src\tools\blender"
  File "..\tools\blender\*.py"
  SetOutPath "$INSTDIR\src\tools\gtkradiant"
  File "..\tools\gtkradiant\*.diff"
  SetOutPath "$INSTDIR\src\tools\gtkradiant\games"
  File "..\tools\gtkradiant\games\*.game"
  SetOutPath "$INSTDIR\src\tools\gtkradiant\plugin"
  SetOutPath "$INSTDIR\src\tools\gtkradiant\plugin\ufoai"
  File "..\tools\gtkradiant\plugin\ufoai\*.cpp"
  File "..\tools\gtkradiant\plugin\ufoai\*.h"
  SetOutPath "$INSTDIR\src\tools\gtkradiant\ufo.game"
  File "..\tools\gtkradiant\ufo.game\*.xml"
  File "..\tools\gtkradiant\ufo.game\*.xlink"
  SetOutPath "$INSTDIR\src\tools\gtkradiant\ufo.game\base"
  File "..\tools\gtkradiant\ufo.game\base\*.def"
  SetOutPath "$INSTDIR\src\tools\qdata"
  File "..\tools\qdata\*.c"
  File "..\tools\qdata\*.h"
  File "..\tools\qdata\makefile"
  File "..\tools\qdata\*.dsp"
  File "..\tools\qdata\*.dsw"
  SetOutPath "$INSTDIR\src\tools\ufo2map"
  File "..\tools\ufo2map\*.h"
  File "..\tools\ufo2map\*.c"
  File "..\tools\ufo2map\*.dev"
  File "..\tools\ufo2map\makefile"
  SetOutPath "$INSTDIR\src\tools\ufo2map\common"
  File "..\tools\ufo2map\common\*.h"
  File "..\tools\ufo2map\common\*.c"

  SetOutPath "$INSTDIR\src"
  File "..\*.cbp"
  File "..\*.dev"
  File "..\*.dsp"
  File "..\*.dsw"
  File "..\*.sln"
  File "..\*.vcproj"
  File "..\*.workspace"
  SetOutPath "$INSTDIR\src\win32"
  File "*.c"
  File "*.h"
  File "installer.nsi"
  File "*.lib"
  File "*.aps"
  File "*.ico"
  File "*.rc"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\ufo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ufo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} ""
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} ""
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) successfully deinstalled."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure that you want to remove $(^Name) and all its data?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
;==============================================================
; TODO: Delete all files from $INSTDIR, too
;==============================================================
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"

  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Website.lnk"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\UFO:Alien Invasion.lnk"

  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
