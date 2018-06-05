import { Component, Injectable, Injector, ReflectiveInjector } from '@angular/core';
import { PropertiesViewerService } from '../propertiesViewer.service';
import { PropertyLabels } from '../Properties/propertyLabels';
import { Vector3Viewer } from '../Properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from '../Properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from '../Properties/boolViewer/boolViewer.component';
import { StringViewer } from '../Properties/stringViewer/stringViewer.component';
import { SliderViewer } from '../Properties/sliderViewer/sliderViewer.component';
import { UndoRedoManagerService, UndoRedoAction } from '../../../assets/Utils/UndoRedo/UndoRedoManager.service';

export class propertyData {
    type: Object;
    injector: Injector;
}
  
export class property {
    name: string;
    propertiesData: Array<propertyData>; 
    constructor() {
        this.propertiesData = new Array<propertyData>();
    }
}


@Injectable()
export class PropertiesViewerVMService {
    private componentTypes : Map<String, Object>;
    private undoRedoService : UndoRedoManagerService; 
    private componentsList : Array<String>;
    private gameObjectProperties : Array<property>;

    constructor(private propertiesViewerService: PropertiesViewerService,
                private injector: Injector,
                private undoRedoManagerService: UndoRedoManagerService) {

        
        this.undoRedoService = UndoRedoManagerService.getInstance();
        this.componentTypes = new Map<String, Object>();
        this.componentsList = Array<String>();
        this.gameObjectProperties = Array<property>();
        this.getGameComponentsStructures();
    }

        // public methods:
    public addComponent(componentName: string) {
        var property = this.addComponentToPropertyViewer(componentName);
        this.UndoRedoAddComponent(componentName);
    }

    public removeComponent(componentName: string) {
        this.UndoRedoRemoveComponent(componentName);
        this.removeComponentFromPropertyViewer(componentName);
    }

    public undoEventTrigger() {
        this.undoRedoManagerService.Undo();
    }

    public redoEventTrigger() {
        this.undoRedoManagerService.Redo();
    }

    
    public getComponentsList() : Array<String> {
        return this.componentsList;
    }

    public getGameObjectProperties() : Array<property> {
        return this.gameObjectProperties;
    }


        // private methods:
    private addComponentToPropertyViewer = (componentName: string) => {
        var componentStructure = this.componentTypes[componentName];
        if(Array.isArray(componentName)) {
            componentStructure = this.componentTypes[componentName['0']];
        }
        else {
            componentStructure = this.componentTypes[componentName];
        }
        var componentProperty = new property();

        componentProperty.name = componentName;
        //Scale, Rotate, Translate
        for(let member of componentStructure) {
            let componentData = new propertyData();
            switch(member.typeName) {
                case "vec3":{
                    this.addVec3SubProperty(member, componentProperty, componentData);
                    break;
                }
                case "vec2": {
                    this.addVec2SubProperty(member, componentProperty, componentData)
                    break;
                }
                case "bool": {
                    this.addBoolSubProperty(member, componentProperty, componentData);
                    break;
                }
                case "string": {
                    this.addStringSubProperty(member, componentProperty, componentData);
                    break;
                }
                case "float": {
                    this.addSliderSubProperty(member, componentProperty, componentData);
                    break;
                }
            }
        }
        this.gameObjectProperties.push(componentProperty);
    }

    public removeComponentFromPropertyViewer = (componentName : string) => {
        var realComponentName;
        if(Array.isArray(componentName)) {
            realComponentName = componentName['0'];
        }
        else {
            realComponentName = componentName;
        }

          // remove the component from the properties viewer
          for(var i = 0; i < this.gameObjectProperties.length; ++i) {
            if(this.gameObjectProperties[i].name == realComponentName) {
                this.gameObjectProperties.splice(i, 1);
            }
        }
    }


    // Gets the structural mark up for all the component types
    private getGameComponentsStructures() {
        this.propertiesViewerService.getComponentsStructures().subscribe(gameComponentStructures => {
                var components = gameComponentStructures['Components'];
                for(var i = 0; i < components.length; ++i){
                    // add the component name to the components list
                    var componentName = components[i]['typeName'];
                    this.componentsList.push(componentName);
                    this.componentTypes[componentName] = components[i]['members'];
                }
            }
        );
    }
    
    
    // Gets a list of all existing component types
    private getComponentTypes() {
        this.propertiesViewerService.getComponentsList().subscribe(componentTypes => {

            }
        );
    }

    private addVec3SubProperty(vec3: any, vec3Property:property, vec3Data: propertyData) {
        let vec3Labels = new Array<string>(3);
        
        for(let i = 0; i < vec3.members.length; ++i) {
          vec3Labels[i] = vec3.members[i].name;
        }
    
        let componentInjector = ReflectiveInjector.resolveAndCreate([{ 
          provide: PropertyLabels, 
          useValue: {
            'name' : vec3.name, 
            'xLabel': vec3Labels[0],
            'yLabel': vec3Labels[1],
            'zLabel': vec3Labels[2]
          }
        }], this.injector)
    
        vec3Data.injector = componentInjector;
        vec3Data.type = Vector3Viewer;
        vec3Property.propertiesData.push(vec3Data);    
      }
    
      private addVec2SubProperty(vec2: any, vec2Property, vec2Data: propertyData) {
        let vec2Labels = new Array<string>(2);
    
        let i = 0;
        for(let i = 0; i < vec2.members.length; ++i) {
          vec2Labels[i] = vec2.members[i].name;
        }
    
        let componentInjector = ReflectiveInjector.resolveAndCreate([{ 
          provide: PropertyLabels, 
          useValue: {
            'name' : vec2.name, 
            'xLabel': vec2Labels[0],
            'yLabel': vec2Labels[1]
          }
        }], this.injector)
    
        vec2Data.injector = componentInjector;
        vec2Data.type = Vector2Viewer;
        vec2Property.propertiesData.push(vec2Data);
      }
    
      private addBoolSubProperty(boolean: any, boolProperty: property, boolData: propertyData) {
        let componentInjector = ReflectiveInjector.resolveAndCreate([{ 
          provide: PropertyLabels, 
          useValue: {
            'name' : boolean.name, 
          }
        }], this.injector);
    
        boolData.injector = componentInjector;
        boolData.type = BoolViewer;
        boolProperty.propertiesData.push(boolData);
      }
    
      private addStringSubProperty(str: any, stringProperty: property, stringData: propertyData) {
        let componentInjector = ReflectiveInjector.resolveAndCreate([{ 
          provide: PropertyLabels, 
          useValue: {
            'name' : str.name, 
          }
        }], this.injector);
    
        stringData.injector = componentInjector;
        stringData.type = StringViewer;
        stringProperty.propertiesData.push(stringData);
      }
    
      private addSliderSubProperty(slider: any, sliderProperty: property, sliderData: propertyData): void {
        let componentInjector = ReflectiveInjector.resolveAndCreate([{ 
          provide: PropertyLabels, 
          useValue: {
            'name' : slider.name, 
          }
        }], this.injector);
    
        sliderData.injector = componentInjector;
        sliderData.type = SliderViewer;
        sliderProperty.propertiesData.push(sliderData);
      }

    // Undo // Redo features
    private UndoRedoAddComponent (componentName : string) {
        var addComponentActions = new UndoRedoAction();
        addComponentActions.setExecuteFunc(this.addComponentToPropertyViewer, componentName);
        addComponentActions.setUnexecuteFunc(this.removeComponentFromPropertyViewer, componentName);
        this.undoRedoService.Add(addComponentActions);
    }

    private UndoRedoRemoveComponent(componentName : string) {
        var removeComponentActions  = new UndoRedoAction();
        removeComponentActions.setExecuteFunc(this.removeComponentFromPropertyViewer, componentName);
        removeComponentActions.setUnexecuteFunc(this.addComponentToPropertyViewer, componentName);
        this.undoRedoService.Add(removeComponentActions);
    }
};