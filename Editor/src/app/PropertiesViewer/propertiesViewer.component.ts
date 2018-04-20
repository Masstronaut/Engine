import { Component, Inject, Injector, ComponentRef, ReflectiveInjector } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import { PropertiesViewerService } from './propertiesViewer.service';
import { PropertyLabels } from './Properties/propertyLabels';
import { Vector3Viewer } from './Properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from './Properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from './Properties/boolViewer/boolViewer.component';
import { StringViewer } from './Properties/stringViewer/stringViewer.component';
import { SliderViewer } from './Properties/sliderViewer/sliderViewer.component';
import * as GoldenLayout from 'golden-layout';

class propertyData {
  type: Object;
  injector: Injector;
}

class property {
  name: string;
  propertiesData: Array<propertyData>; 
  constructor() {
    this.propertiesData = new Array<propertyData>();
  }
}


@Component({
  selector: 'properties-viewer',
  templateUrl: './propertiesViewer.html',
  styleUrls: ['./propertiesViewer.scss']    
})

export class PropertiesViewerComponent implements GlOnResize, GlOnHide, GlOnShow {
  //List of components queried from the engine
  componentsList: Object;
  gameObjectComponents: Object;
  
  //Array of properties currently to be viewed
  currentGameObjectProperties: Array<property>;
  
  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container,
              private service: PropertiesViewerService,
              private injector: Injector) {
    this.currentGameObjectProperties = new Array<property>();
  }

  ngOnInit() {
    this.getGameObjectComponents();
    this.getAllComponents();
  }

  //Gets all components on a game object
  getGameObjectComponents() {
    this.service.getGameObjectComponents().subscribe(gameObjectComponents => {
        this.gameObjectComponents = gameObjectComponents;
      });
  }

  //Gets all existing components from the game engine
  getAllComponents() {
    this.service.getComponents().subscribe(components => {
        this.componentsList = components;
      });
  }


  addComponent(componentName: string) {
    
    let component = this.componentsList['Components'];
    let componentProperty = new property();
    for(let property of component) {
      //Transform, Sprite,...
      if(property.typeName == componentName){
        componentProperty.name = componentName;
        //Scale, Rotate, Translate
        for(let member of property.members) {
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
        this.currentGameObjectProperties.push(componentProperty);
        return; 
      }
    }
  }
 
  
  addVec3SubProperty(vec3: any, vec3Property:property, vec3Data: propertyData) {
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

  addVec2SubProperty(vec2: any, vec2Property, vec2Data: propertyData) {
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

  addBoolSubProperty(boolean: any, boolProperty: property, boolData: propertyData) {
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

  addStringSubProperty(str: any, stringProperty: property, stringData: propertyData) {
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

  addSliderSubProperty(slider: any, sliderProperty: property, sliderData: propertyData): void {
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

  public onInput(e: Event): void {
    
    this.container.extendState({
      value: (<HTMLInputElement>e.target).value
    });

    console.log('Game Object Properties state saved.');
  }

  public glOnResize(): void {
    console.log('Game Object Properties Resizing!');
  }

  public glOnShow(): void {
    console.log('Game Object Properties Showing!');
  }

  public glOnHide(): void {
    console.log('Game Object Properties Hiding!');
  }

}