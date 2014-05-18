Welcome to the VaQuoleUI source code!
=====================================

VaQuoleUI is the Web UI framework plugin for [Unreal Engine 4](https://www.unrealengine.com/) proudly based on [Qt framework](http://qt-project.org/).

The plugin includes:

* Actor component that renders website content into 2D texture
* GameViewport and PlayerController classes to help with input setup
* Simple HTML example of HUD with JavaScript callbacks
* Source of Qt-based library, that handles all under the hood

Plugin is actively used in [SeaCraft](http://seacraft.sc) project development.

Current version: **0.2 Alpha 7**

![SCREENSHOT](SCREENSHOT.jpg)


Features
--------

Some key things you should know about the VaOceanUI:

* Cross-platform in theory, tested on and pre-built for Windows only
* Can be used to integrate **any** Qt widgets
* Supports translucent backgrounds
* Can be embedded both via Texture2D and Material Instance parameter (to be used on scene)
* Custom JavaScript code can be evaluated to control the page content
* Callback from JS scripts are processed by Exec functions
* Shouldn't be used for production yet! Alpha-version, yo!

Futher improvements are on the way. Check the [Issues](https://github.com/ufna/VaQuoleUI/issues) tab to know more.


HowTo
-----

> How to open local files with UIComponent?

You should set the link as _**vaquole://**Relative/Path/To/MyPage.html_, where the path is relative to your project dir (where .uproject file is situated).

> How to open _https_ links?

To use the SSL connection (https) you have to rebuild the Qt-based project with its support. Pre-built binaries work without encryption only.


Installation
------------

1. Download the **plugin binaries** for the latest release.
1. Copy plugin binaries **exactly** to your project directory.
1. Compile your game project normally. Unreal Build Tool will detect the plugins and compile them as dependencies to your game.
1. Launch the editor (or the game). Plugin will be initially disabled, but you can turn it on in the editor UI.
1. Open the Plugins Editor (Window -> Plugins), search for VaQuoleUI plugin (you can find it in the **UI** section) and enable it by clicking the check box.
1. Restart the Editor. The plugin will be automatically loaded at startup.


Legal info
----------

Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere.

Unreal® Engine, Copyright 1998 – 2014, Epic Games, Inc. All rights reserved.

