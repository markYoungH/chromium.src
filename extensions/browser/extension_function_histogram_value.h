// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EXTENSION_FUNCTION_HISTOGRAM_VALUE_H_
#define EXTENSIONS_BROWSER_EXTENSION_FUNCTION_HISTOGRAM_VALUE_H_

namespace extensions {
namespace functions {

// Short version:
//  *Never* reorder or delete entries in the |HistogramValue| enumeration.
//  When creating a new extension function, add a new entry at the end of the
//  enum, just prior to ENUM_BOUNDARY.
//
// Long version:
//  This enumeration is used to associate a unique integer value to each
//  extension function so that their usage can be recorded in histogram charts.
//  Given we want the values recorded in the these charts to remain stable over
//  time for comparison purposes, once an entry has been added to the
//  enumeration, it should never be removed or moved to another spot in the
//  enum.
//
//  Here are instructions how to manage entries depending on what you are trying
//  to achieve.
//
//  1) Creating a new extension function:
//
//      Add a new entry at the end of the |HistogramValue| enum. The name of the
//      entry should follow this algorithm:
//      a) Take the string value passed as first argument to
//         DECLARE_EXTENSION_FUNCTION.
//      b) Replace '.' with '_'.
//      c) Make all letters uppercase.
//
//      Example: "tabs.create" -> TABS_CREATE
//
//  2) Deleting an existing function:
//
//      Given an existing entry should *never* be removed from this enumeration,
//      it is recommended to add a "DELETED_" prefix to the existing entry.
//
//  3) Renaming an existing function:
//
//      There are 2 options, depending if you want to keep accruing data in the
//      *existing* histogram stream or in a *new* one.
//
//      a) If you want keep recording usages of the extension function in the
//         *existing* histogram stream, simply rename the enum entry to match
//         the new extension function name, following the same naming rule as
//         mentioned in 1). The enum entry will keep the same underlying integer
//         value, so the same histogram stream will be used for recording
//         usages.
//
//      b) If you want start recording usages of the extension function to in a
//         *new* histogram stream, follow the instructions in step 1) and 2)
//         above. This will effectively deprecate the old histogram stream and
//         creates a new one for the new function name.
//
enum HistogramValue {
  UNKNOWN = 0,
  WEBNAVIGATION_GETALLFRAMES,
  BROWSINGDATA_REMOVEWEBSQL,
  ALARMS_CREATE,
  FILEBROWSERPRIVATE_REMOVEFILEWATCH,
  COOKIES_GET,
  FONTSETTINGS_GETMINIMUMFONTSIZE,
  CHROMEOSINFOPRIVATE_GET,
  BOOKMARKMANAGERPRIVATE_CUT,
  TABS_CAPTUREVISIBLETAB,
  MANAGEMENT_SETENABLED,
  HISTORY_DELETEALL,
  STORAGE_GET,
  SOCKET_SETKEEPALIVE,
  DOWNLOADS_CANCEL,
  BOOKMARKS_CREATE,
  BOOKMARKS_UPDATE,
  FILEBROWSERPRIVATE_GETDRIVEFILES,
  TERMINALPRIVATE_ONTERMINALRESIZE,
  DELETED_FILEBROWSERPRIVATE_REQUESTDIRECTORYREFRESH,
  BLUETOOTH_GETADAPTERSTATE,
  FILEBROWSERPRIVATE_CANCELFILETRANSFERS,
  FILEBROWSERPRIVATE_PINDRIVEFILE,
  SOCKET_WRITE,
  OMNIBOX_SETDEFAULTSUGGESTION,
  TTS_SPEAK,
  WALLPAPERPRIVATE_RESTOREMINIMIZEDWINDOWS,
  BROWSINGDATA_REMOVEHISTORY,
  DELETED_FILEBROWSERPRIVATE_ISFULLSCREEN,
  AUTOTESTPRIVATE_LOGOUT,
  EXPERIMENTAL_HISTORY_GETMOSTVISITED,
  DELETED_BLUETOOTH_DISCONNECT,
  DELETED_BLUETOOTH_SETOUTOFBANDPAIRINGDATA,
  BOOKMARKMANAGERPRIVATE_CANPASTE,
  AUTOTESTPRIVATE_RESTART,
  USB_CLAIMINTERFACE,
  MEDIAPLAYERPRIVATE_SETWINDOWHEIGHT,
  DELETED_EXPERIMENTAL_PROCESSES_GETPROCESSINFO,
  HISTORY_GETVISITS,
  SOCKET_BIND,
  TABS_MOVE,
  SOCKET_DISCONNECT,
  FILESYSTEM_GETWRITABLEENTRY,
  SYNCFILESYSTEM_REQUESTFILESYSTEM,
  COMMANDS_GETALL,
  EXPERIMENTAL_DISCOVERY_REMOVESUGGESTION,
  VIRTUALKEYBOARDPRIVATE_SENDKEYEVENT,
  BOOKMARKMANAGERPRIVATE_GETSUBTREE,
  DELETED_EXPERIMENTAL_RLZ_RECORDPRODUCTEVENT,
  BOOKMARKS_GETRECENT,
  APP_CURRENTWINDOWINTERNAL_SETBOUNDS,
  CLOUDPRINTPRIVATE_SETUPCONNECTOR,
  SERIAL_SETCONTROLSIGNALS,
  DELETED_FILEBROWSERPRIVATE_SETLASTMODIFIED,
  IDLE_SETDETECTIONINTERVAL,
  FILEBROWSERPRIVATE_GETFILETASKS,
  WEBSTOREPRIVATE_GETSTORELOGIN,
  SYSTEMPRIVATE_GETINCOGNITOMODEAVAILABILITY,
  IDLTEST_SENDARRAYBUFFERVIEW,
  SOCKET_SETNODELAY,
  APP_CURRENTWINDOWINTERNAL_SHOW,
  WEBSTOREPRIVATE_GETBROWSERLOGIN,
  EXPERIMENTAL_IDENTITY_GETAUTHTOKEN,
  DELETED_SYSTEMINFO_DISPLAY_GETDISPLAYINFO,
  BROWSINGDATA_REMOVEPLUGINDATA,
  SOCKET_LISTEN,
  MEDIAGALLERIES_GETMEDIAFILESYSTEMS,
  DOWNLOADS_OPEN,
  TABS_EXECUTESCRIPT,
  SYNCFILESYSTEM_GETUSAGEANDQUOTA,
  INPUTMETHODPRIVATE_GET,
  USB_CLOSEDEVICE,
  TTS_STOP,
  DELETED_SERIAL_GETPORTS,
  DELETED_FILEBROWSERPRIVATE_CLEARDRIVECACHE,
  SERIAL_GETCONTROLSIGNALS,
  DEVELOPERPRIVATE_ENABLE,
  FILEBROWSERPRIVATE_GETDRIVEFILEPROPERTIES,
  USB_FINDDEVICES,
  BOOKMARKMANAGERPRIVATE_DROP,
  DELETED_FILEBROWSERPRIVATE_GETFILETRANSFERS,
  INPUT_IME_SETMENUITEMS,
  BOOKMARKS_EXPORT,
  HISTORY_SEARCH,
  TTSENGINE_SENDTTSEVENT,
  EXPERIMENTAL_ACCESSIBILITY_GETALERTSFORTAB,
  BOOKMARKS_IMPORT,
  SYNCFILESYSTEM_DELETEFILESYSTEM,
  DEBUGGER_SENDCOMMAND,
  DEBUGGER_DETACH,
  METRICSPRIVATE_RECORDSMALLCOUNT,
  APP_CURRENTWINDOWINTERNAL_MINIMIZE,
  DEVELOPERPRIVATE_AUTOUPDATE,
  DNS_RESOLVE,
  DELETED_EXPERIMENTAL_SYSTEMINFO_MEMORY_GET,
  HISTORY_ADDURL,
  TABS_GET,
  BROWSERACTION_SETBADGETEXT,
  TABS_RELOAD,
  WINDOWS_CREATE,
  DEVELOPERPRIVATE_LOADUNPACKED,
  DELETED_DOWNLOADS_SETDESTINATION,
  DELETED_EXPERIMENTAL_PROCESSES_GETPROCESSIDFORTAB,
  BOOKMARKS_GETCHILDREN,
  BROWSERACTION_GETTITLE,
  TERMINALPRIVATE_OPENTERMINALPROCESS,
  DELETED_SERIAL_CLOSE,
  CONTEXTMENUS_REMOVE,
  FILEBROWSERPRIVATE_REQUESTFILESYSTEM,
  ECHOPRIVATE_GETREGISTRATIONCODE,
  TABS_GETCURRENT,
  FONTSETTINGS_CLEARDEFAULTFIXEDFONTSIZE,
  MEDIAPLAYERPRIVATE_CLOSEWINDOW,
  WEBREQUESTINTERNAL_ADDEVENTLISTENER,
  CLOUDPRINTPRIVATE_GETPRINTERS,
  STORAGE_SET,
  FONTSETTINGS_GETDEFAULTFONTSIZE,
  EXTENSION_SETUPDATEURLDATA,
  DELETED_SERIAL_WRITE,
  IDLE_QUERYSTATE,
  DELETED_EXPERIMENTAL_RLZ_GETACCESSPOINTRLZ,
  WEBSTOREPRIVATE_SETSTORELOGIN,
  PAGEACTIONS_ENABLEFORTAB,
  COOKIES_SET,
  CONTENTSETTINGS_SET,
  CONTEXTMENUS_REMOVEALL,
  TABS_INSERTCSS,
  WEBREQUEST_HANDLERBEHAVIORCHANGED,
  INPUT_IME_SETCURSORPOSITION,
  OMNIBOX_SENDSUGGESTIONS,
  SYSTEMINDICATOR_ENABLE,
  EVENTS_GETRULES,
  BOOKMARKMANAGERPRIVATE_COPY,
  SOCKET_RECVFROM,
  TABS_GETALLINWINDOW,
  CONTEXTMENUS_UPDATE,
  BOOKMARKS_SEARCH,
  EXPERIMENTAL_APP_CLEARALLNOTIFICATIONS,
  DELETED_BLUETOOTH_GETLOCALOUTOFBANDPAIRINGDATA,
  SYSTEMPRIVATE_GETUPDATESTATUS,
  FONTSETTINGS_CLEARMINIMUMFONTSIZE,
  DELETED_FILEBROWSERPRIVATE_GETFILELOCATIONS,
  EXPERIMENTAL_DISCOVERY_SUGGEST,
  FILEBROWSERPRIVATE_SETDEFAULTTASK,
  BROWSERACTION_GETBADGETEXT,
  APP_CURRENTWINDOWINTERNAL_HIDE,
  SOCKET_CONNECT,
  BOOKMARKS_GETSUBTREE,
  HISTORY_DELETEURL,
  DELETED_EXPERIMENTAL_MEDIAGALLERIES_ASSEMBLEMEDIAFILE,
  BOOKMARKMANAGERPRIVATE_STARTDRAG,
  BROWSINGDATA_REMOVEPASSWORDS,
  DOWNLOADS_DRAG,
  INPUT_IME_SETCOMPOSITION,
  METRICSPRIVATE_RECORDUSERACTION,
  USB_RELEASEINTERFACE,
  PAGEACTION_GETPOPUP,
  DELETED_SCRIPTBADGE_GETATTENTION,
  FONTSETTINGS_GETFONTLIST,
  PERMISSIONS_CONTAINS,
  DELETED_SCRIPTBADGE_GETPOPUP,
  EXPERIMENTAL_ACCESSIBILITY_GETFOCUSEDCONTROL,
  DEVELOPERPRIVATE_GETSTRINGS,
  METRICSPRIVATE_RECORDMEDIUMCOUNT,
  MANAGEMENT_GET,
  PERMISSIONS_GETALL,
  DOWNLOADS_SHOW,
  DELETED_EXPERIMENTAL_RLZ_CLEARPRODUCTSTATE,
  TABS_REMOVE,
  MANAGEMENT_GETPERMISSIONWARNINGSBYID,
  WINDOWS_GET,
  FILEBROWSERPRIVATE_EXECUTETASK,
  TTS_GETVOICES,
  MANAGEMENT_GETALL,
  MANAGEMENT_GETPERMISSIONWARNINGSBYMANIFEST,
  APP_CURRENTWINDOWINTERNAL_CLEARATTENTION,
  AUTOTESTPRIVATE_SHUTDOWN,
  FONTSETTINGS_CLEARDEFAULTFONTSIZE,
  BOOKMARKS_GETTREE,
  FILEBROWSERPRIVATE_SELECTFILES,
  RUNTIME_GETBACKGROUNDPAGE,
  DELETED_EXPERIMENTAL_RECORD_REPLAYURLS,
  WEBSTOREPRIVATE_COMPLETEINSTALL,
  DELETED_EXPERIMENTAL_SPEECHINPUT_START,
  COOKIES_GETALL,
  DOWNLOADS_GETFILEICON,
  PAGEACTION_GETTITLE,
  BROWSINGDATA_REMOVE,
  DELETED_SERIAL_OPEN,
  FILESYSTEM_GETDISPLAYPATH,
  FILEBROWSERPRIVATE_FORMATVOLUME,
  BOOKMARKS_GET,
  DELETED_MANAGEDMODEPRIVATE_GET,
  ALARMS_CLEAR,
  SYNCFILESYSTEM_GETFILESYNCSTATUS,
  SOCKET_GETINFO,
  WEBSTOREPRIVATE_INSTALLBUNDLE,
  BROWSERACTION_ENABLE,
  METRICSPRIVATE_RECORDMEDIUMTIME,
  PAGEACTION_SETTITLE,
  CLOUDPRINTPRIVATE_GETHOSTNAME,
  CONTENTSETTINGS_GETRESOURCEIDENTIFIERS,
  SOCKET_CREATE,
  DEVELOPERPRIVATE_RELOAD,
  FILEBROWSERPRIVATE_GETVOLUMEMETADATALIST,
  APP_RUNTIME_POSTINTENTRESPONSE,
  DELETED_MANAGEDMODEPRIVATE_SETPOLICY,
  WEBSTOREPRIVATE_BEGININSTALLWITHMANIFEST3,
  WALLPAPERPRIVATE_SETWALLPAPER,
  USB_CONTROLTRANSFER,
  DELETED_EXPERIMENTAL_SPEECHINPUT_STOP,
  USB_BULKTRANSFER,
  DELETED_FILEBROWSERPRIVATE_GETVOLUMEMETADATA,
  PAGECAPTURE_SAVEASMHTML,
  EXTENSION_ISALLOWEDINCOGNITOACCESS,
  BROWSINGDATA_REMOVEAPPCACHE,
  APP_CURRENTWINDOWINTERNAL_DRAWATTENTION,
  METRICSPRIVATE_RECORDCOUNT,
  USB_INTERRUPTTRANSFER,
  TYPES_CHROMESETTING_CLEAR,
  INPUT_IME_COMMITTEXT,
  IDLTEST_SENDARRAYBUFFER,
  WALLPAPERPRIVATE_SETWALLPAPERIFEXISTS,
  SOCKET_ACCEPT,
  WEBNAVIGATION_GETFRAME,
  EXPERIMENTAL_POWER_RELEASEKEEPAWAKE,
  APP_CURRENTWINDOWINTERNAL_SETICON,
  PUSHMESSAGING_GETCHANNELID,
  EXPERIMENTAL_INFOBARS_SHOW,
  INPUT_IME_SETCANDIDATEWINDOWPROPERTIES,
  METRICSPRIVATE_RECORDPERCENTAGE,
  TYPES_CHROMESETTING_GET,
  WINDOWS_GETLASTFOCUSED,
  DELETED_MANAGEDMODEPRIVATE_GETPOLICY,
  STORAGE_CLEAR,
  STORAGE_GETBYTESINUSE,
  TABS_QUERY,
  PAGEACTION_SETPOPUP,
  DEVELOPERPRIVATE_INSPECT,
  DOWNLOADS_SEARCH,
  FONTSETTINGS_CLEARFONT,
  WINDOWS_UPDATE,
  BOOKMARKMANAGERPRIVATE_CANOPENNEWWINDOWS,
  SERIAL_FLUSH,
  BROWSERACTION_SETTITLE,
  BOOKMARKMANAGERPRIVATE_CANEDIT,
  WALLPAPERPRIVATE_SETCUSTOMWALLPAPER,
  BOOKMARKS_REMOVE,
  INPUT_IME_SETCANDIDATES,
  TERMINALPRIVATE_CLOSETERMINALPROCESS,
  HISTORY_DELETERANGE,
  IDLTEST_GETARRAYBUFFER,
  TERMINALPRIVATE_SENDINPUT,
  TABS_HIGHLIGHT,
  BLUETOOTH_STARTDISCOVERY,
  FILEBROWSERPRIVATE_SELECTFILE,
  WINDOWS_GETCURRENT,
  DEBUGGER_ATTACH,
  WALLPAPERPRIVATE_SAVETHUMBNAIL,
  INPUT_IME_KEYEVENTHANDLED,
  FONTSETTINGS_SETDEFAULTFONTSIZE,
  RUNTIME_REQUESTUPDATECHECK,
  PAGEACTION_SETICON,
  BROWSERACTION_SETBADGEBACKGROUNDCOLOR,
  DEVELOPERPRIVATE_GETITEMSINFO,
  BLUETOOTH_STOPDISCOVERY,
  COOKIES_REMOVE,
  DELETED_EXPERIMENTAL_RLZ_SENDFINANCIALPING,
  TABCAPTURE_GETCAPTUREDTABS,
  WINDOWS_REMOVE,
  WALLPAPERPRIVATE_GETOFFLINEWALLPAPERLIST,
  BROWSERACTION_GETBADGEBACKGROUNDCOLOR,
  PAGEACTIONS_DISABLEFORTAB,
  DEVELOPERPRIVATE_ALLOWFILEACCESS,
  FILEBROWSERPRIVATE_REMOVEMOUNT,
  DELETED_BLUETOOTH_CONNECT,
  TABCAPTURE_CAPTURE,
  NOTIFICATIONS_CREATE,
  TABS_DUPLICATE,
  DELETED_BLUETOOTH_WRITE,
  PAGEACTION_SHOW,
  WALLPAPERPRIVATE_GETTHUMBNAIL,
  DOWNLOADS_PAUSE,
  PERMISSIONS_REQUEST,
  TOPSITES_GET,
  BROWSINGDATA_REMOVEDOWNLOADS,
  BROWSINGDATA_REMOVELOCALSTORAGE,
  FILEBROWSERHANDLERINTERNAL_SELECTFILE,
  INPUT_IME_UPDATEMENUITEMS,
  FILEBROWSERPRIVATE_GETSTRINGS,
  CONTENTSETTINGS_GET,
  FONTSETTINGS_SETDEFAULTFIXEDFONTSIZE,
  EXPERIMENTAL_APP_NOTIFY,
  METRICSPRIVATE_RECORDLONGTIME,
  SOCKET_READ,
  DELETED_EXPERIMENTAL_PROCESSES_TERMINATE,
  METRICSPRIVATE_RECORDTIME,
  BOOKMARKMANAGERPRIVATE_GETSTRINGS,
  USB_ISOCHRONOUSTRANSFER,
  PERMISSIONS_REMOVE,
  MANAGEMENT_UNINSTALL,
  I18N_GETACCEPTLANGUAGES,
  MANAGEMENT_LAUNCHAPP,
  INPUT_IME_CLEARCOMPOSITION,
  ALARMS_GETALL,
  DIAL_DISCOVERNOW,
  TYPES_CHROMESETTING_SET,
  BROWSERACTION_SETICON,
  EXPERIMENTAL_ACCESSIBILITY_SETACCESSIBILITYENABLED,
  DELETED_FILEBROWSERPRIVATE_VIEWFILES,
  DELETED_BLUETOOTH_GETSERVICES,
  TABS_UPDATE,
  BROWSINGDATA_REMOVEFORMDATA,
  DELETED_FILEBROWSERPRIVATE_RELOADDRIVE,
  ALARMS_GET,
  BROWSINGDATA_REMOVEINDEXEDDB,
  FILEBROWSERPRIVATE_ADDFILEWATCH,
  CONTENTSETTINGS_CLEAR,
  FILEBROWSERPRIVATE_GETPREFERENCES,
  BOOKMARKMANAGERPRIVATE_PASTE,
  FILESYSTEM_ISWRITABLEENTRY,
  USB_SETINTERFACEALTERNATESETTING,
  FONTSETTINGS_SETMINIMUMFONTSIZE,
  BROWSERACTION_GETPOPUP,
  SOCKET_DESTROY,
  BLUETOOTH_GETDEVICES,
  ALARMS_CLEARALL,
  FONTSETTINGS_GETDEFAULTFIXEDFONTSIZE,
  FILEBROWSERPRIVATE_ZIPSELECTION,
  SYSTEMINDICATOR_DISABLE,
  DELETED_SCRIPTBADGE_SETPOPUP,
  EXTENSION_ISALLOWEDFILESCHEMEACCESS,
  EXPERIMENTAL_IDENTITY_LAUNCHWEBAUTHFLOW,
  FILEBROWSERPRIVATE_GETDRIVECONNECTIONSTATE,
  TABS_DETECTLANGUAGE,
  METRICSPRIVATE_RECORDVALUE,
  BOOKMARKMANAGERPRIVATE_SORTCHILDREN,
  DELETED_SERIAL_READ,
  APP_CURRENTWINDOWINTERNAL_MAXIMIZE,
  EXPERIMENTAL_DISCOVERY_CLEARALLSUGGESTIONS,
  DELETED_MANAGEDMODEPRIVATE_ENTER,
  DELETED_FILEBROWSERPRIVATE_TRANSFERFILE,
  BROWSERACTION_SETPOPUP,
  TABS_GETSELECTED,
  FONTSETTINGS_GETFONT,
  DELETED_BLUETOOTH_READ,
  WEBREQUESTINTERNAL_EVENTHANDLED,
  EVENTS_ADDRULES,
  CONTEXTMENUS_CREATE,
  MEDIAPLAYERPRIVATE_GETPLAYLIST,
  DOWNLOADS_ERASE,
  DELETED_EXPERIMENTAL_RECORD_CAPTUREURLS,
  TTS_ISSPEAKING,
  BOOKMARKS_REMOVETREE,
  FILEBROWSERPRIVATE_SEARCHDRIVE,
  DELETED_EXPERIMENTAL_SYSTEMINFO_CPU_GET,
  FILEBROWSERPRIVATE_SETPREFERENCES,
  FONTSETTINGS_SETFONT,
  SOCKET_GETNETWORKLIST,
  BOOKMARKS_MOVE,
  WALLPAPERPRIVATE_MINIMIZEINACTIVEWINDOWS,
  STORAGE_REMOVE,
  AUTOTESTPRIVATE_LOGINSTATUS,
  TABS_CREATE,
  FILEBROWSERPRIVATE_CANCELDIALOG,
  BROWSINGDATA_REMOVECOOKIES,
  FILESYSTEM_CHOOSEENTRY,
  MEDIAPLAYERPRIVATE_PLAY,
  WEBSTOREPRIVATE_GETWEBGLSTATUS,
  SOCKET_SENDTO,
  BROWSINGDATA_REMOVEFILESYSTEMS,
  WALLPAPERPRIVATE_GETSTRINGS,
  BROWSINGDATA_REMOVECACHE,
  BOOKMARKMANAGERPRIVATE_RECORDLAUNCH,
  BROWSERACTION_DISABLE,
  DELETED_EXPERIMENTAL_SPEECHINPUT_ISRECORDING,
  APP_WINDOW_CREATE,
  RUNTIME_RELOAD,
  EXPERIMENTAL_POWER_REQUESTKEEPAWAKE,
  SYSTEMINDICATOR_SETICON,
  FILEBROWSERPRIVATE_ADDMOUNT,
  APP_CURRENTWINDOWINTERNAL_FOCUS,
  EVENTS_REMOVERULES,
  DOWNLOADS_DOWNLOAD,
  WINDOWS_GETALL,
  DELETED_FILEBROWSERPRIVATE_TOGGLEFULLSCREEN,
  APP_CURRENTWINDOWINTERNAL_RESTORE,
  DELETED_WEBSOCKETPROXYPRIVATE_GETPASSPORTFORTCP,
  PAGEACTION_HIDE,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_GET,
  DOWNLOADS_ACCEPTDANGER,
  DELETED_WEBSOCKETPROXYPRIVATE_GETURLFORTCP,
  FILEBROWSERPRIVATE_GETSIZESTATS,
  DOWNLOADS_RESUME,
  COOKIES_GETALLCOOKIESTORES,
  MEDIAGALLERIESPRIVATE_ADDGALLERYWATCH,
  MEDIAGALLERIESPRIVATE_REMOVEGALLERYWATCH,
  WEBVIEW_EXECUTESCRIPT,
  NOTIFICATIONS_UPDATE,
  NOTIFICATIONS_CLEAR,
  DELETED_SESSIONRESTORE_GETRECENTLYCLOSED,
  DELETED_SESSIONRESTORE_RESTORE,
  MANAGEMENT_UNINSTALLSELF,
  ECHOPRIVATE_GETOOBETIMESTAMP,
  FILEBROWSERPRIVATE_VALIDATEPATHNAMELENGTH,
  BROWSINGDATA_SETTINGS,
  WEBSTOREPRIVATE_GETISLAUNCHERENABLED,
  NETWORKINGPRIVATE_GETPROPERTIES,
  NETWORKINGPRIVATE_GETVISIBLENETWORKS,
  NETWORKINGPRIVATE_STARTCONNECT,
  NETWORKINGPRIVATE_STARTDISCONNECT,
  MEDIAGALLERIESPRIVATE_GETALLGALLERYWATCH,
  MEDIAGALLERIESPRIVATE_REMOVEALLGALLERYWATCH,
  FILEBROWSERPRIVATE_SEARCHDRIVEMETADATA,
  DELETED_ECHOPRIVATE_CHECKALLOWREDEEMOFFERS,
  DELETED_MEDIAGALLERIESPRIVATE_EJECTDEVICE,
  FILEBROWSERPRIVATE_LOGOUTUSERFORREAUTHENTICATION,
  DEVELOPERPRIVATE_CHOOSEPATH,
  DEVELOPERPRIVATE_PACKDIRECTORY,
  NETWORKINGPRIVATE_VERIFYDESTINATION,
  NETWORKINGPRIVATE_VERIFYANDENCRYPTCREDENTIALS,
  NETWORKINGPRIVATE_VERIFYANDENCRYPTDATA,
  DEVELOPERPRIVATE_RESTART,
  DEVELOPERPRIVATE_ALLOWINCOGNITO,
  INPUT_IME_DELETESURROUNDINGTEXT,
  DELETED_FILEBROWSERPRIVATE_OPENNEWWINDOW,
  CLOUDPRINTPRIVATE_GETCLIENTID,
  ECHOPRIVATE_GETUSERCONSENT,
  SYNCFILESYSTEM_SETCONFLICTRESOLUTIONPOLICY,
  SYNCFILESYSTEM_GETCONFLICTRESOLUTIONPOLICY,
  NETWORKINGPRIVATE_SETPROPERTIES,
  NETWORKINGPRIVATE_GETSTATE,
  POWER_REQUESTKEEPAWAKE,
  POWER_RELEASEKEEPAWAKE,
  WALLPAPERPRIVATE_SETCUSTOMWALLPAPERLAYOUT,
  DOWNLOADSINTERNAL_DETERMINEFILENAME,
  SYNCFILESYSTEM_GETFILESYNCSTATUSES,
  DELETED_MEDIAGALLERIESPRIVATE_GETHANDLERS,
  WALLPAPERPRIVATE_RESETWALLPAPER,
  DEVELOPERPRIVATE_PERMISSIONS,
  WEBSTOREPRIVATE_ENABLEAPPLAUNCHER,
  APP_CURRENTWINDOWINTERNAL_FULLSCREEN,
  DEVELOPERPRIVATE_LOADUNPACKEDCROS,
  NETWORKINGPRIVATE_REQUESTNETWORKSCAN,
  ENTERPRISE_PLATFORMKEYSPRIVATE_CHALLENGEMACHINEKEY,
  ENTERPRISE_PLATFORMKEYSPRIVATE_CHALLENGEUSERKEY,
  DEVELOPERPRIVATE_LOADPROJECTTOSYNCFS,
  DEVELOPERPRIVATE_GETPROJECTSINFO,
  DEVELOPERPRIVATE_LOADPROJECT,
  COMMANDLINEPRIVATE_HASSWITCH,
  NETWORKINGPRIVATE_GETMANAGEDPROPERTIES,
  LOCATION_WATCHLOCATION,
  LOCATION_CLEARWATCH,
  DELETED_BLUETOOTH_ADDPROFILE,
  DELETED_BLUETOOTH_REMOVEPROFILE,
  DELETED_BLUETOOTH_GETPROFILES,
  EXPERIMENTAL_IDENTITY_REMOVECACHEDAUTHTOKEN,
  AUDIO_GETINFO,
  AUDIO_SETACTIVEDEVICES,
  AUDIO_SETPROPERTIES,
  USB_RESETDEVICE,
  SOCKET_MULTICAST_JOIN_GROUP,
  SOCKET_MULTICAST_LEAVE_GROUP,
  SOCKET_MULTICAST_SET_TIME_TO_LIVE,
  SOCKET_MULTICAST_SET_LOOPBACK_MODE,
  SOCKET_MULTICAST_GET_JOINED_GROUPS,
  EXPERIMENTAL_ACCESSIBILITY_SETNATIVEACCESSIBILITYENABLED,
  RUNTIME_GETPLATFORMINFO,
  WEBVIEW_INSERTCSS,
  METRICSPRIVATE_GETISCRASHRECORDINGENABLED,
  IDENTITYPRIVATE_GETSTRINGS,
  NOTIFICATIONS_GET_ALL,
  USB_LISTINTERFACES,
  FILESYSTEM_RETAINENTRY,
  FILESYSTEM_ISRESTORABLE,
  FILESYSTEM_RESTOREENTRY,
  RUNTIME_SETUNINSTALLURL,
  INPUTMETHODPRIVATE_STARTIME,
  MUSICMANAGERPRIVATE_GETDEVICEID,
  TTS_PAUSE,
  TTS_RESUME,
  DELETED_SYSTEMINFO_CPU_GET,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_ADDWATCH,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_REMOVEWATCH,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_GETALLWATCH,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_REMOVEALLWATCH,
  DELETED_SYSTEMINFO_MEMORY_GET,
  ACTIVITYLOGPRIVATE_GETEXTENSIONACTIVITIES,
  RUNTIME_GETPACKAGEDIRECTORYENTRY,
  DELETED_SYSTEMINFO_DISPLAY_SETDISPLAYPROPERTIES,
  FEEDBACKPRIVATE_GETUSEREMAIL,
  FEEDBACKPRIVATE_GETSYSTEMINFORMATION,
  FEEDBACKPRIVATE_SENDFEEDBACK,
  VIRTUALKEYBOARDPRIVATE_INSERTTEXT,
  DIAGNOSTICS_SENDPACKET,
  METRICSPRIVATE_GETFIELDTRIAL,
  FILEBROWSERPRIVATE_ZOOM,
  WEBVIEW_GO,
  WEBSTOREPRIVATE_ISININCOGNITOMODEFUNCTION,
  FILEBROWSERPRIVATE_REQUESTACCESSTOKEN,
  WEBVIEW_STOP,
  WEBVIEW_RELOAD,
  WEBVIEW_TERMINATE,
  TYPES_PRIVATE_CHROMEDIRECTSETTING_GET,
  TYPES_PRIVATE_CHROMEDIRECTSETTING_SET,
  TYPES_PRIVATE_CHROMEDIRECTSETTING_CLEAR,
  DELETED_EXPERIMENTAL_SYSTEMINFO_STORAGE_EJECTDEVICE,
  SYSTEM_CPU_GETINFO,
  BOOKMARKMANAGERPRIVATE_REMOVETREES,
  SYSTEM_DISPLAY_GETINFO,
  SYSTEM_DISPLAY_SETDISPLAYPROPERTIES,
  SYSTEM_MEMORY_GETINFO,
  FILEBROWSERPRIVATE_GETSHAREURL,
  SYSTEM_STORAGE_GETINFO,
  SYSTEM_STORAGE_EJECTDEVICE,
  DELETED_SYSTEM_STORAGE_ADDAVAILABLECAPACITYWATCH,
  DELETED_SYSTEM_STORAGE_REMOVEAVAILABLECAPACITYWATCH,
  DELETED_SYSTEM_STORAGE_GETALLAVAILABLECAPACITYWATCHES,
  DELETED_SYSTEM_STORAGE_REMOVEALLAVAILABLECAPACITYWATCHES,
  DOWNLOADS_REMOVEFILE,
  DOWNLOADS_SHOWDEFAULTFOLDER,
  INFOBARS_SHOW,
  DOWNLOADS_SETSHELFENABLED,
  IMAGEWRITER_WRITEFROMURL,
  IMAGEWRITER_WRITEFROMFILE,
  IMAGEWRITER_CANCELWRITE,
  IMAGEWRITER_DESTROYPARTITIONS,
  FEEDBACKPRIVATE_GETSTRINGS,
  LOGPRIVATE_GETHISTORICAL,
  VIRTUALKEYBOARDPRIVATE_MOVECURSOR,
  METRICSPRIVATE_GETVARIATIONPARAMS,
  WEBVIEW_SETPERMISSION,
  DESKTOPCAPTURE_CHOOSEDESKTOPMEDIA,
  APP_CURRENTWINDOWINTERNAL_SETSHAPE,
  PROCESSES_GETPROCESSINFO,
  PROCESSES_GETPROCESSIDFORTAB,
  PROCESSES_TERMINATE,
  SOCKETS_UDP_CREATE,
  SOCKETS_UDP_UPDATE,
  SOCKETS_UDP_BIND,
  SOCKETS_UDP_SEND,
  SOCKETS_UDP_CLOSE,
  SOCKETS_UDP_GETINFO,
  SOCKETS_UDP_GETSOCKETS,
  SOCKETS_UDP_JOINGROUP,
  SOCKETS_UDP_LEAVEGROUP,
  SOCKETS_UDP_SETMULTICASTTIMETOLIVE,
  SOCKETS_UDP_SETMULTICASTLOOPBACKMODE,
  SOCKETS_UDP_GETJOINEDGROUPS,
  SIGNED_IN_DEVICES_GET,
  AUTOTESTPRIVATE_SIMULATEASANMEMORYBUG,
  WEBVIEW_CLEARDATA,
  SESSIONS_GETRECENTLYCLOSED,
  SESSIONS_GETDEVICES,
  SESSIONS_RESTORE,
  SYNCFILESYSTEM_GETSERVICESTATUS,
  ECHOPRIVATE_SETOFFERINFO,
  ECHOPRIVATE_GETOFFERINFO,
  DEVELOPERPRIVATE_ISPROFILEMANAGED,
  FILEBROWSERPRIVATE_INSTALLWEBSTOREITEM,
  FILEBROWSERPRIVATE_STARTCOPY,
  FILEBROWSERPRIVATE_CANCELCOPY,
  NETWORKINGPRIVATE_CREATENETWORK,
  BRAILLEDISPLAYPRIVATE_GETDISPLAYSTATE,
  BRAILLEDISPLAYPRIVATE_WRITEDOTS,
  USB_GETDEVICES,
  USB_REQUESTACCESS,
  USB_OPENDEVICE,
  ACTIVITYLOGPRIVATE_DELETEDATABASE,
  ACTIVITYLOGPRIVATE_DELETEURLS,
  FILEBROWSERPRIVATE_REQUESTWEBSTOREACCESSTOKEN,
  IMAGEWRITER_LISTREMOVABLESTORAGEDEVICES,
  WALLPAPER_SETWALLPAPER,
  VIRTUALKEYBOARDPRIVATE_HIDEKEYBOARD,
  AUTOTESTPRIVATE_LOCKSCREEN,
  WEBRTCLOGGINGPRIVATE_SETMETADATA,
  WEBRTCLOGGINGPRIVATE_START,
  WEBRTCLOGGINGPRIVATE_SETUPLOADONRENDERCLOSE,
  WEBRTCLOGGINGPRIVATE_STOP,
  WEBRTCLOGGINGPRIVATE_UPLOAD,
  WEBRTCLOGGINGPRIVATE_DISCARD,
  WEBVIEW_OVERRIDEUSERAGENT,
  PRINCIPALSPRIVATE_SHOWAVATARBUBBLE,
  PRINCIPALSPRIVATE_SIGNOUT,
  CAST_CHANNEL_OPEN,
  CAST_CHANNEL_SEND,
  CAST_CHANNEL_CLOSE,
  RUNTIME_RESTART,
  DESKTOPCAPTURE_CANCELCHOOSEDESKTOPMEDIA,
  APP_CURRENTWINDOWINTERNAL_SETALWAYSONTOP,
  SOCKETS_TCP_CREATE,
  SOCKETS_TCP_UPDATE,
  SOCKETS_TCP_SETPAUSED,
  SOCKETS_TCP_SETKEEPALIVE,
  SOCKETS_TCP_SETNODELAY,
  SOCKETS_TCP_CONNECT,
  SOCKETS_TCP_DISCONNECT,
  SOCKETS_TCP_SEND,
  SOCKETS_TCP_CLOSE,
  SOCKETS_TCP_GETINFO,
  SOCKETS_TCP_GETSOCKETS,
  NETWORKINGPRIVATE_GETENABLEDNETWORKTYPES,
  NETWORKINGPRIVATE_ENABLENETWORKTYPE,
  NETWORKINGPRIVATE_DISABLENETWORKTYPE,
  SOCKETS_TCP_SERVER_CREATE,
  SOCKETS_TCP_SERVER_UPDATE,
  SOCKETS_TCP_SERVER_SETPAUSED,
  SOCKETS_TCP_SERVER_LISTEN,
  SOCKETS_TCP_SERVER_DISCONNECT,
  SOCKETS_TCP_SERVER_CLOSE,
  SOCKETS_TCP_SERVER_GETINFO,
  SOCKETS_TCP_SERVER_GETSOCKETS,
  SYSTEM_STORAGE_GETAVAILABLECAPACITY,
  BROWSERACTION_OPEN_POPUP,
  WEBRTC_AUDIO_PRIVATE_GET_SINKS,
  WEBRTC_AUDIO_PRIVATE_GET_ACTIVE_SINK,
  WEBRTC_AUDIO_PRIVATE_SET_ACTIVE_SINK,
  WEBRTC_AUDIO_PRIVATE_GET_ASSOCIATED_SINK,
  VIRTUALKEYBOARDPRIVATE_KEYBOARDLOADED,
  DELETED_APP_CURRENTWINDOWINTERNAL_SETMINWIDTH,
  DELETED_APP_CURRENTWINDOWINTERNAL_SETMINHEIGHT,
  DELETED_APP_CURRENTWINDOWINTERNAL_SETMAXWIDTH,
  DELETED_APP_CURRENTWINDOWINTERNAL_SETMAXHEIGHT,
  SYSTEMPRIVATE_GETAPIKEY,
  CHROMEOSINFOPRIVATE_SET,
  BOOKMARKMANAGERPRIVATE_GETMETAINFO,
  BOOKMARKMANAGERPRIVATE_SETMETAINFO,
  FILESYSTEMPROVIDER_MOUNT,
  AUTOTESTPRIVATE_GETEXTENSIONSINFO,
  SCREENLOCKPRIVATE_GETLOCKED,
  SCREENLOCKPRIVATE_SETLOCKED,
  SCREENLOCKPRIVATE_SHOWMESSAGE,
  FEEDBACKPRIVATE_GETHISTOGRAMS,
  SYSTEM_NETWORK_GETNETWORKINTERFACES,
  SERIAL_GETDEVICES,
  SERIAL_UPDATE,
  SERIAL_SETPAUSED,
  SERIAL_GETINFO,
  SERIAL_GETCONNECTIONS,
  SERIAL_SEND,
  GCM_REGISTER,
  GCM_SEND,
  SERIAL_CONNECT,
  SERIAL_DISCONNECT,
  MEDIAGALLERIES_GETALLMEDIAFILESYSTEMMETADATA,
  FIRSTRUNPRIVATE_GETLOCALIZEDSTRINGS,
  FIRSTRUNPRIVATE_LAUNCHTUTORIAL,
  SOCKETS_UDP_SETPAUSED,
  WEBVIEW_CAPTUREVISIBLEREGION,
  MEDIAGALLERIES_GETMETADATA,
  INPUT_IME_SENDKEYEVENTS,
  VIRTUALKEYBOARDPRIVATE_LOCKKEYBOARD,
  SCREENLOCKPRIVATE_SHOWCUSTOMICON,
  INPUT_IME_HIDEINPUTVIEW,
  BOOKMARKMANAGERPRIVATE_UNDO,
  BOOKMARKMANAGERPRIVATE_REDO,
  BOOKMARKMANAGERPRIVATE_UNDOINFO,
  BOOKMARKMANAGERPRIVATE_REDOINFO,
  MEDIAGALLERIES_ADDUSERSELECTEDFOLDER,
  PREFERENCESPRIVATE_GETSYNCCATEGORIESWITHOUTPASSPHRASE,
  READINGLISTPRIVATE_ADDENTRY,
  READINGLISTPRIVATE_REMOVEENTRY,
  READINGLISTPRIVATE_GETENTRIES,
  MEDIAGALLERIES_STARTMEDIASCAN,
  MEDIAGALLERIES_CANCELMEDIASCAN,
  MEDIAGALLERIES_ADDSCANRESULTS,
  LOGPRIVATE_STARTNETINTERNALSWATCH,
  LOGPRIVATE_STOPNETINTERNALSWATCH,
  FILEBROWSERPRIVATE_GETPROFILES,
  FILEBROWSERPRIVATE_VISITDESKTOP,
  VIRTUALKEYBOARDPRIVATE_GETKEYBOARDCONFIG,
  HID_GETDEVICES,
  HID_CONNECT,
  HID_DISCONNECT,
  HID_RECEIVE,
  HID_SEND,
  HID_RECEIVEFEATUREREPORT,
  HID_SENDFEATUREREPORT,
  HOTWORDPRIVATE_SETENABLED,
  HOTWORDPRIVATE_GETSTATUS,
  APP_CURRENTWINDOWINTERNAL_SETBADGEICON,
  APP_CURRENTWINDOWINTERNAL_CLEARBADGE,
  WEBVIEW_SETZOOM,
  WEBVIEW_GETZOOM,
  DEVELOPERPRIVATE_REQUESTFILESOURCE,
  DEVELOPERPRIVATE_OPENDEVTOOLS,
  ACTIVITYLOGPRIVATE_DELETEACTIVITIES,
  NETWORKINGPRIVATE_SETWIFITDLSENABLEDSTATE,
  NETWORKINGPRIVATE_GETWIFITDLSSTATUS,
  SCREENLOCKPRIVATE_HIDECUSTOMICON,
  SCREENLOCKPRIVATE_SETAUTHTYPE,
  SCREENLOCKPRIVATE_GETAUTHTYPE,
  SCREENLOCKPRIVATE_ACCEPTAUTHATTEMPT,
  WEBVIEW_FIND,
  WEBVIEW_STOPFINDING,
  WEBVIEW_CONTEXTMENUSCREATE,
  WEBVIEW_CONTEXTMENUSUPDATE,
  WEBVIEW_CONTEXTMENUSREMOVE,
  WEBVIEW_CONTEXTMENUSREMOVEALL,
  AUTOMATIONINTERNAL_ENABLETAB,
  APP_CURRENTWINDOWINTERNAL_SETSIZECONSTRAINTS,
  BLUETOOTH_GETDEVICE,
  GCM_UNREGISTER,
  FILEBROWSERPRIVATE_REQUESTDRIVESHARE,
  METRICSPRIVATE_RECORDSPARSEVALUE,
  HOTWORDPRIVATE_SETAUDIOLOGGINGENABLED,
  BLUETOOTHPRIVATE_SETADAPTERSTATE,
  BLUETOOTHPRIVATE_ENABLEPAIRING,
  BLUETOOTHPRIVATE_DISABLEPAIRING,
  BLUETOOTHPRIVATE_SETPAIRINGRESPONSE,
  NETWORKINGPRIVATE_GETCAPTIVEPORTALSTATUS,
  AUTOMATIONINTERNAL_PERFORMACTION,
  DELETED_BLUETOOTH_UPDATE_SOCKET,
  DELETED_BLUETOOTH_SET_SOCKET_PAUSED,
  DELETED_BLUETOOTH_GET_SOCKET,
  DELETED_BLUETOOTH_GET_SOCKETS,
  FILESYSTEMPROVIDER_UNMOUNT,
  FILESYSTEMPROVIDERINTERNAL_UNMOUNTREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_UNMOUNTREQUESTEDERROR,
  MEDIAGALLERIES_DROPPERMISSIONFORMEDIAFILESYSTEM,
  WEBCAMPRIVATE_SET,
  WEBCAMPRIVATE_RESET,
  WEBCAMPRIVATE_GET,
  BLUETOOTHLOWENERGY_GETSERVICE,
  BLUETOOTHLOWENERGY_GETSERVICES,
  BLUETOOTHLOWENERGY_GETCHARACTERISTIC,
  BLUETOOTHLOWENERGY_GETCHARACTERISTICS,
  BLUETOOTHLOWENERGY_GETINCLUDEDSERVICES,
  BLUETOOTHLOWENERGY_GETDESCRIPTOR,
  BLUETOOTHLOWENERGY_GETDESCRIPTORS,
  BLUETOOTHLOWENERGY_READCHARACTERISTICVALUE,
  BLUETOOTHLOWENERGY_WRITECHARACTERISTICVALUE,
  BLUETOOTHLOWENERGY_READDESCRIPTORVALUE,
  BLUETOOTHLOWENERGY_WRITEDESCRIPTORVALUE,
  BOOKMARKMANAGERPRIVATE_CREATEWITHMETAINFO,
  BOOKMARKMANAGERPRIVATE_UPDATEMETAINFO,
  BLUETOOTHSOCKET_CREATE,
  BLUETOOTHSOCKET_UPDATE,
  BLUETOOTHSOCKET_SETPAUSED,
  BLUETOOTHSOCKET_LISTENUSINGRFCOMM,
  BLUETOOTHSOCKET_LISTENUSINGINSECURERFCOMM,
  BLUETOOTHSOCKET_LISTENUSINGL2CAP,
  BLUETOOTHSOCKET_CONNECT,
  BLUETOOTHSOCKET_DISCONNECT,
  BLUETOOTHSOCKET_CLOSE,
  BLUETOOTHSOCKET_SEND,
  BLUETOOTHSOCKET_GETINFO,
  BLUETOOTHSOCKET_GETSOCKETS,
  WEBSTOREPRIVATE_SIGNINFUNCTION,
  SHELL_CREATEWINDOW,
  FILESYSTEMPROVIDERINTERNAL_GETMETADATAREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_GETMETADATAREQUESTEDERROR,
  BROWSER_OPENTAB,
  MANAGEMENT_CREATEAPPSHORTCUT,
  WEBVIEW_SHOWCONTEXTMENU,
  WEBRTCLOGGINGPRIVATE_STARTRTPDUMP,
  WEBRTCLOGGINGPRIVATE_STOPRTPDUMP,
  AUTOMATIONINTERNAL_ENABLEDESKTOP,
  HOTWORDPRIVATE_SETHOTWORDSESSIONSTATE,
  HOTWORDPRIVATE_NOTIFYHOTWORDRECOGNITION,
  FILESYSTEMPROVIDERINTERNAL_READDIRECTORYREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_READDIRECTORYREQUESTEDERROR,
  LEDGER_BATCHEXECUTE,
  FILESYSTEMPROVIDERINTERNAL_OPENFILEREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_OPENFILEREQUESTEDERROR,
  FILESYSTEMPROVIDERINTERNAL_CLOSEFILEREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_CLOSEFILEREQUESTEDERROR,
  SYNCEDNOTIFICATIONSPRIVATE_GETINITIALDATA,
  SYNCEDNOTIFICATIONSPRIVATE_UPDATENOTIFICATION,
  SYNCEDNOTIFICATIONSPRIVATE_SETRENDERCONTEXT,
  IDENTITY_GETACCOUNTS,
  FILEBROWSERPRIVATE_RESOLVEISOLATEDENTRIES,
  FILESYSTEMPROVIDERINTERNAL_READFILEREQUESTEDSUCCESS,
  DELETED_FILESYSTEMPROVIDERINTERNAL_READFILEREQUESTEDERROR,
  NETWORKINGPRIVATE_GETNETWORKS,
  WEBVIEW_SETNAME,
  ENTERPRISE_PLATFORMKEYSINTERNAL_GENERATEKEY,
  ENTERPRISE_PLATFORMKEYSINTERNAL_SIGN,
  ENTERPRISE_PLATFORMKEYSINTERNAL_GETTOKENS,
  ENTERPRISE_PLATFORMKEYS_GETCERTIFICATES,
  ENTERPRISE_PLATFORMKEYS_IMPORTCERTIFICATE,
  ENTERPRISE_PLATFORMKEYS_REMOVECERTIFICATE,
  FILEBROWSERPRIVATE_OPENINSPECTOR,
  STREAMSPRIVATE_ABORT,
  MANAGEMENT_SETLAUNCHTYPE,
  MANAGEMENT_GENERATEAPPFORLINK,
  GUESTVIEWINTERNAL_ALLOCATEINSTANCEID,
  WEBVIEW_NAVIGATE,
  INPUTMETHODPRIVATE_GETCURRENTINPUTMETHOD,
  INPUTMETHODPRIVATE_SETCURRENTINPUTMETHOD,
  INPUTMETHODPRIVATE_GETINPUTMETHODS,
  IDENTITY_GETPROFILEUSERINFO,
  VIRTUALKEYBOARDPRIVATE_OPENSETTINGS,
  BLUETOOTHLOWENERGY_CONNECT,
  BLUETOOTHLOWENERGY_DISCONNECT,
  WEBSTOREPRIVATE_GETEPHEMERALAPPSENABLED,
  WEBSTOREPRIVATE_LAUNCHEPHEMERALAPP,
  FILESYSTEMPROVIDERINTERNAL_OPERATIONREQUESTEDERROR,
  // Last entry: Add new entries above and ensure to update
  // tools/metrics/histograms/histograms/histograms.xml.
  ENUM_BOUNDARY
};

}  // namespace functions
}  // namespace extensions

#endif  // EXTENSIONS_BROWSER_EXTENSION_FUNCTION_HISTOGRAM_VALUE_H_
