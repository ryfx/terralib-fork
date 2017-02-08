#This script is used to compile release versions of the TerraView plugins
#------------------------------------------------------------------------
all: debug release

clean_all: clean clean_dbg

release: tview wmstheme wfstheme tview_plg tdkplugin edition_plg pdi_plg

debug: tview_dbg wmstheme_dbg wfstheme_dbg tview_plg_dbg tdkplugin_dbg edition_plg_dbg pdi_plg_dbg

tview:
	cd ../build/qt; make;
	
tview_plg:
	qmake MAKEFILE=Makefile.Release; make -f Makefile.Release && make -f Makefile.Release install;
	
wmstheme:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wmsclient; qmake teogc_wms MAKEFILE=MakeOGC.Release; make -f MakeOGC.Release; \
		qmake wmsclient.pro MAKEFILE=Makefile.Release; make -f Makefile.Release && make -f Makefile.Release install; \
	fi

wfstheme:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wfsclient; qmake wfsplugin.pro MAKEFILE=MakeOGC.Release; make -f MakeOGC.Release; \
		qmake wfsclient.pro MAKEFILE=Makefile.Release; make -f Makefile.Release && make -f Makefile.Release install; \
	fi

tdkplugin:
	if \
		test -f ../../applications/terrabasePlugins/pluginTdk/Makefile; \
	 then \
		cd ../../applications/terrabasePlugins/pluginTdk; make release; \
	fi
	
edition_plg:
	if \
		test -f ../../applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/TdkBasicEditionPlugin; qmake MAKEFILE=Makefile.Release TDK_PROJECT_TYPE=PLUGIN; make -f Makefile.Release && make -f Makefile.Release install; \
	fi
	
pdi_plg:
	if \
		test -f ../../applications/terrabasePlugins/pdiPlugin/pdiPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/pdiPlugin; qmake MAKEFILE=Makefile.Release TDK_PROJECT_TYPE=PLUGIN; make -f Makefile.Release && make -f Makefile.Release install; \
	fi
	
tview_dbg:
	cd ../build/qt; make debug;
	
tview_plg_dbg:
	qmake MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG; make -f Makefile.Debug; make -f Makefile.Debug install;
	
wmstheme_dbg:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wmsclient;  \
		qmake wmsclient.pro MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG; make -f Makefile.Debug;  make -f Makefile.Debug install; \
	fi

wfstheme_dbg:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wfsclient;  \
		qmake wfsclient.pro MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG; make -f Makefile.Debug;  make -f Makefile.Debug install; \
	fi

tdkplugin_dbg:
	if \
		test -f ../../applications/terrabasePlugins/pluginTdk/Makefile; \
	 then \
		cd ../../applications/terrabasePlugins/pluginTdk; make debug; \
	fi
	
edition_plg_dbg:
	if \
		test -f ../../applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/TdkBasicEditionPlugin; qmake MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG TDK_PROJECT_TYPE=PLUGIN;  make -f Makefile.Debug && make -f Makefile.Debug install; \
	fi
	
pdi_plg_dbg:
	if \
		test -f ../../applications/terrabasePlugins/pdiPlugin/pdiPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/pdiPlugin; qmake MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG TDK_PROJECT_TYPE=PLUGIN; make -f Makefile.Debug && make -f Makefile.Debug install; \
	fi
	
clean_dbg: clean_tview_dbg clean_plug_dbg

clean_plug_dbg: clean_tdkplugin_dbg clean_edition_plg_dbg clean_wmstheme_dbg clean_wfstheme_dbg clean_tview_plg_dbg

clean_tview_dbg:
	cd ../build/qt; make clean_debug;

clean_tview_plg_dbg:	
	make -f Makefile.Debug distclean; make -f Makefile.Debug uninstall;

clean_wmstheme_dbg:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wmsclient;  \
		make -f Makefile.Debug distclean;   \
	fi

clean_wfstheme_dbg:
	if \
		test -f ../../terraogc/build/linux/all/all.pro; \
	 then \
		cd ./wfsclient;  \
		make -f Makefile.Debug distclean;   \
	fi

clean_tdkplugin_dbg:
	if \
		test -f ../../applications/terrabasePlugins/pluginTdk/Makefile; \
	 then \
		cd ../../applications/terrabasePlugins/pluginTdk; make clean_dbg; \
	fi
	
clean_edition_plg_dbg:
	if \
		test -f ../../applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/TdkBasicEditionPlugin; make -f Makefile.Debug distclean; make -f Makefile.Debug uninstall; \
	fi

clean_plug: clean_tdkplugin clean_edition_plg clean_wmstheme clean_tview_plg

clean: clean_tview clean_plug

clean_tview:
	cd ../build/qt; make clean;

clean_tview_plg:	
	make -f Makefile.Release distclean; make -f Makefile.Release uninstall;

clean_wmstheme:
	if \
		test -f ../extensions/terraOGC/build/linux/buildAll; \
	 then \
		cd ./wmsclient; make -f MakeOGC.Release distclean; \
		make -f Makefile.Release distclean;  make -f Makefile.Release uninstall; \
	fi

clean_wfstheme:
	if \
		test -f ../extensions/terraOGC/build/linux/buildAll; \
	 then \
		cd ./wfsclient; make -f MakeOGC.Release distclean; \
		make -f Makefile.Release distclean;  make -f Makefile.Release uninstall; \
	fi

clean_tdkplugin:
	if \
		test -f ../../applications/terrabasePlugins/pluginTdk/Makefile; \
	 then \
		cd ../../applications/terrabasePlugins/pluginTdk; make clean; \
	fi
	
clean_edition_plg:
	if \
		test -f ../../applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin.pro; \
	 then \
		cd ../../applications/terrabasePlugins/TdkBasicEditionPlugin; make -f Makefile.Release distclean;  make -f Makefile.Release uninstall; \
	fi
