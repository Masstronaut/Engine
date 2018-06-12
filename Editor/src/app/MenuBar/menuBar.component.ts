import { Component, OnInit, ViewChild } from '@angular/core';
import { MatMenuTrigger } from '@angular/material/menu';
import { ConsoleViewerComponent } from '../ConsoleViewer/consoleViewer.component';
import { HierachyViewerComponent } from '../HierachyViewer/hierachyViewer.component';
import { PropertiesViewerComponent } from '../PropertiesViewer/propertiesViewer.component';
import { SceneViewerComponent } from '../SceneViewer/sceneViewer.component';
import { ProjectViewerComponent } from '../ProjectViewer/projectViewer.component';

import { 
    GoldenLayoutModule, 
    GoldenLayoutConfiguration, 
    GoldenLayoutService,
    GoldenLayoutContainer
  } from 'ng-golden-layout';
  import * as GoldenLayout from 'golden-layout';

@Component({
    selector: 'menu-bar',
    templateUrl: './menuBar.html',
    styleUrls: ['./menuBar.scss']
})


export class MenuBarComponent {
    // File dropdown options:
    gFileOptions : Array<Object>;
    gEditOptions : Array<Object>;
    gViewOptions : Array<Object>;

    gDragSourcesInit : Boolean = false;
    public static gConsoleViewExists : Boolean = false;
    public static gHierarchyViewExists : Boolean = true;
    public static gPropertiesViewExists : Boolean = true;
    public static gSceneViewExists : Boolean = true;
    public static gProjectViewExists : Boolean = true;

    private gClosedConsoleOnStart : Boolean = false;

      // make sure the views are properly set up with their event handlers
    private gConsoleContainerInit : Boolean = false;
    private gHierarchyContainerInit : Boolean = false;
    private gPropertiesContainerInit : Boolean = false;
    private gSceneContainerInit : Boolean = false;
    private gProjectContainerInit : Boolean = false;

    // File Options for Menu Bar:
    MenuButton_NewProjectTrigger() {
        console.log("New Project Menu Button!");
    }

    MenuButton_OpenProjectTrigger() {
        console.log("Open Project Menu Button!");
    }

    MenuButton_SaveProjectTrigger() {
        console.log("Save Project Menu Button!");
    }

    // Edit Options for Menu Bar
    MenuButton_CutTrigger() {
        console.log("Cut Menu Button!");
    }

    MenuButton_CopyTrigger() {
        console.log("Copy Menu Button!");
    }

    MenuButton_PasteTrigger() {
        console.log("Paste Menu Button!");
    }

    MenuButton_DuplicateTrigger() {
        console.log("Duplicate Menu Button!");
    }

    MenuButton_DeleteTrigger() {
        console.log("Delete Menu Button!");
    }

    // View Options for Menu Bar
    MenuButton_ViewProperties() {
        var propertiesViewExists = MenuBarComponent.gPropertiesViewExists;
        var containerManager = PropertiesViewerComponent.containerManager;

        if(propertiesViewExists) {
            return;
        }

        var newPropertiesViewConfig : GoldenLayout.ComponentConfig = {
            type : "component",
            width : 20,
            height : 20,
            componentName : "Properties"
        }

        containerManager.layoutManager.root.contentItems[0].addChild(newPropertiesViewConfig);
        MenuBarComponent.gPropertiesViewExists = true;
    }

    MenuButton_ViewHierarchy() {
        var hierarchyExists = MenuBarComponent.gHierarchyViewExists;
        var containerManager = HierachyViewerComponent.containerManager;

        if(hierarchyExists) {
            return;
        }

        var newHierarchyViewConfig : GoldenLayout.ComponentConfig = {
            type : "component",
            width : 20,
            height : 20,
            componentName : "Hierarchy"
        }

        containerManager.layoutManager.root.contentItems[0].addChild(newHierarchyViewConfig);
        MenuBarComponent.gHierarchyViewExists = true;
    }

    MenuButton_ViewScene() {
        var sceneExists = MenuBarComponent.gSceneViewExists;
        var containerManager = SceneViewerComponent.containerManager;

        if(sceneExists) {
            return;
        }

        var newSceneViewConfig : GoldenLayout.ComponentConfig = {
            type : "component",
            width : 20,
            height : 20,
            componentName : "Scene"
        }

        containerManager.layoutManager.root.contentItems[0].addChild(newSceneViewConfig);
        MenuBarComponent.gSceneViewExists = true;
    }

    public MenuButton_ViewProject() {
        var projectExists = MenuBarComponent.gProjectViewExists;
        var containerManager = ProjectViewerComponent.containerManager;

        if(projectExists) {
            return;
        }
        
        var newProjectViewConfig : GoldenLayout.ComponentConfig = {
            type : "component",
            width : 20,
            height : 20,
            componentName : "Project"
        }
        
        containerManager.layoutManager.root.contentItems[0].addChild(newProjectViewConfig);
        MenuBarComponent.gProjectViewExists = true;
    }

    public MenuButton_ViewConsole() {
        var consoleExists = MenuBarComponent.gConsoleViewExists;
        var containerManager = ConsoleViewerComponent.containerManager;

        if(consoleExists) {
            return;
        }
        
        var newConsoleViewConfig : GoldenLayout.ComponentConfig = {
            type : "component",
            width : 20,
            height : 20,
            componentName : "Console"
        }
        
        containerManager.layoutManager.root.contentItems[0].addChild(newConsoleViewConfig);
        MenuBarComponent.gConsoleViewExists = true;
    }


    private SetupMenuBarOptions() {
        this.gFileOptions = [
            {
                optionName: "New Project",
                onclickTrigger: this.MenuButton_NewProjectTrigger
            },
            {
                optionName: "Open Project",
                onclickTrigger: this.MenuButton_OpenProjectTrigger
            },
            {
                optionName: "Save Project",
                onclickTrigger: this.MenuButton_SaveProjectTrigger
            }
        ];

        // set up the Edit Function Menu, sets up the name for each button as well as the triggers
        this.gEditOptions = [
            {
                optionName: "Cut",
                onclickTrigger: this.MenuButton_CutTrigger
            },
            {
                optionName: "Copy",
                onclickTrigger: this.MenuButton_CopyTrigger
            },
            {
                optionName: "Paste",
                onclickTrigger: this.MenuButton_PasteTrigger
            },
            {
                optionName: "Duplicate",
                onclickTrigger: this.MenuButton_DuplicateTrigger
            },
            {
                optionName: "Delete",
                onclickTrigger: this.MenuButton_DeleteTrigger
            }
        ];

        this.gViewOptions = [
            {
                optionName: "Properties",
                onclickTrigger: this.MenuButton_ViewProperties
            },
            {
                optionName: "Hierarchy",
                onclickTrigger: this.MenuButton_ViewHierarchy
            },
            {
                optionName: "Scene",
                onclickTrigger: this.MenuButton_ViewScene
            },
            {
                optionName: "Project",
                onclickTrigger: this.MenuButton_ViewProject
            },
            {
                optionName: "Console",
                onclickTrigger: this.MenuButton_ViewConsole
            }
        ];
    }

    public showConsole() {
        console.log("Console is now showing!");
    }


    public getConsoleContainer() : GoldenLayout.Container {
        return ConsoleViewerComponent.containerManager;
    }


    public getHierarchyContainer() : GoldenLayout.Container {
        return HierachyViewerComponent.containerManager;
    }

    public getPropertiesContainer() : GoldenLayout.Container {
        return PropertiesViewerComponent.containerManager;
    }

    public getSceneContainer() : GoldenLayout.Container {
        return SceneViewerComponent.containerManager;
    }

    public getProjectContainer() : GoldenLayout.Container {
        return ProjectViewerComponent.containerManager;
    }

    public InitViewDragSources() {
        if(this.gDragSourcesInit == true) {
            return;
        }

        // TODO: Find a way to get drag to work with the views being created normally.
        //       Currently, when you drag a new window, it will create duplicates. 
        //       Stop this from happening or remove the feature entirely
        // var consoleViewItem = {
        //     type: 'component',
        //     width: 20,
        //     height: 20,
        //     componentName: 'Console',
        // };
        //var consoleViewEle = document.getElementById("ConsoleView");
        //  var dragsource = consoleContainer.layoutManager.createDragSource(consoleViewEle, consoleViewItem);

          // make sure this function only happens once when the 'View' button is clicked
        this.gDragSourcesInit = true;
    }

    constructor() {
        this.SetupMenuBarOptions();
    }

    public ngAfterContentChecked() {
        var consoleContainer = this.getConsoleContainer();
        var hierachyContainer = this.getHierarchyContainer();
        var propertiesContainer = this.getPropertiesContainer();
        var sceneContainer = this.getSceneContainer();
        var projectContainer = this.getProjectContainer();

        if(consoleContainer && this.gConsoleContainerInit == false) {
            // intercept the itemDestroyed event and toggle the ConsoleExists flag, to open the window later on request.
            consoleContainer.layoutManager.on('itemDestroyed', function(event) {
                if(event.componentName && event.componentName == "Console") {
                    MenuBarComponent.gConsoleViewExists = false;
                }
            }, null);

            this.gConsoleContainerInit = true;
        }

        if(hierachyContainer && this.gHierarchyContainerInit == false) {
            hierachyContainer.layoutManager.on('itemDestroyed', function(event) {
                if(event.componentName && event.componentName == "Hierarchy") {
                    MenuBarComponent.gHierarchyViewExists = false;
                }
            }, null);

            this.gHierarchyContainerInit = true;
        }

        if(propertiesContainer && this.gPropertiesContainerInit == false) {
            propertiesContainer.layoutManager.on('itemDestroyed', function(event) {
                if(event.componentName && event.componentName == "Properties") {
                    MenuBarComponent.gPropertiesViewExists = false;
                }
            }, null);

            this.gPropertiesContainerInit = true;
        }

        if(sceneContainer && this.gSceneContainerInit == false) {
            sceneContainer.layoutManager.on('itemDestroyed', function(event) {
                if(event.componentName && event.componentName == "Scene") {
                    MenuBarComponent.gSceneViewExists = false;
                }
            }, null);

            this.gSceneContainerInit = true;
        }

        if(projectContainer && this.gProjectContainerInit == false) {
            projectContainer.layoutManager.on('itemDestroyed', function(event) {
                if(event.componentName && event.componentName == "Project") {
                    MenuBarComponent.gProjectViewExists = false;
                }
            }, null);

            this.gProjectContainerInit = true;
        }

          // close the hidden console that was made on start up. Needed for getting a reference to the console's container
        if(consoleContainer && consoleContainer.height !== null && consoleContainer.width !== null && this.gClosedConsoleOnStart == false) {
            consoleContainer.close();
            this.gClosedConsoleOnStart = true;
        }
    }
}
