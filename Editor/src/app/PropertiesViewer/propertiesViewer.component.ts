import { Component, Inject, Injector, ComponentRef, ReflectiveInjector } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import { PropertiesViewerService } from './propertiesViewer.service';
import { PropertyLabels } from './Properties/propertyLabels';
import { Vector3Viewer } from './Properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from './Properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from './Properties/boolViewer/boolViewer.component';
import { StringViewer } from './Properties/stringViewer/stringViewer.component';
import { SliderViewer } from './Properties/sliderViewer/sliderViewer.component';
import { PropertiesViewerVMService } from './VMLayer/propertiesViewerVM.service';

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
  componentsList: Array<String>;
  
  //Array of properties currently to be viewed
  currentGameObjectProperties: Array<property>;
  
  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container,
              private propertiesViewerVMService: PropertiesViewerVMService,
              private service: PropertiesViewerService,
              private injector: Injector) {

    this.currentGameObjectProperties = new Array<property>();
  }

  ngOnInit() {
  }

  getComponentsList() : Array<String> {
    return this.propertiesViewerVMService.getComponentsList();
  }

  addComponent(componentName: string) {
    var componentProperty = this.propertiesViewerVMService.addComponentToPropertyViewer(componentName);
    this.currentGameObjectProperties.push(componentProperty);
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