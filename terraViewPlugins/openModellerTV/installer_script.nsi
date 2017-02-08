; defines added by tim to streamline / softcode install process
; Make sure the the following two vars are correct for your system
!define PRODUCT_VERSION "1.0.0"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "openModellerTV Plugin"
!define PRODUCT_PUBLISHER "http://www.dpi.inpe.br"
!define PRODUCT_WEB_SITE "http://www.dpi.inpe.br/~alexcj/openModellerTV.htm"

!define BUILD_DIR "C:\dev\omTvPlugin\terralib321\terraViewPlugins\openModellerTV\libs\Release\bin"
!define INSTALL_DIR "$PROGRAMFILES\Terraview321\plugins"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "openModellerTVPlugin_${PRODUCT_VERSION}_installer.exe"
InstallDir "${INSTALL_DIR}"

ShowInstDetails show
ShowUnInstDetails show


Section "Application" SEC01
  ;this section is mandatory
  SectionIn RO
  ;Added by Tim to install for all users not just the logged in user..
  ;make sure this is at the top of the section
  SetShellVarContext all
  
  SetOutPath "$INSTDIR\"
  File /r "${BUILD_DIR}\*"
SectionEnd
