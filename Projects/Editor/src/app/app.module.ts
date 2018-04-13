import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { PropertyLabels } from './propertiesViewer/properties/propertyLabels';
import { FormsModule } from '@angular/forms';
import { MatMenuModule } from '@angular/material/menu';
import { PropertiesViewerService } from './propertiesViewer/propertiesViewer.service';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatButtonModule } from '@angular/material/button';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatSlideToggleModule } from '@angular/material/slide-toggle';
import { MatExpansionPanel, MatExpansionModule } from '@angular/material/expansion';
import { MatSliderModule } from '@angular/material/slider';
import { TestPanelComponent } from './test-panel.component';
import { PropertiesViewerComponent } from  './propertiesViewer/propertiesViewer.component';
import { SceneViewerComponent } from './sceneViewer/scene.component';
import { ProjectViewerComponent } from './projectViewer/projectViewer.component';
import { HierachyViewerComponent } from './hierachyViewer/hierachyViewer.component';
import { MenuBarComponent } from './menuBar/menuBar.component';
import { MatIconModule } from '@angular/material/icon';
import { StringViewer } from './propertiesViewer/properties/stringViewer/stringViewer.component';
import { SliderViewer } from './propertiesViewer/properties/sliderViewer/sliderViewer.component';
import { Vector3Viewer } from './propertiesViewer/properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from './propertiesViewer/properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from './propertiesViewer/properties/boolViewer/boolViewer.component';

import 'hammerjs';

import { 
  GoldenLayoutModule, 
  GoldenLayoutConfiguration, 
  // DEFAULT_LOCAL_STORAGE_STATE_STORE_PROVIDER 
} from 'ng-golden-layout';
import * as GoldenLayout from 'golden-layout';
import * as Jquery from 'jquery';
import { AppComponent } from './app.component';

const goldenLayoutConfig: GoldenLayoutConfiguration = {
  components: [
    {
      component: TestPanelComponent,
      componentName: 'test-panel'
    },
    {
      component: SceneViewerComponent,
      componentName: 'Scene'
    },
    {
      component: ProjectViewerComponent,
      componentName: 'Project' 
    },
    {
      component: PropertiesViewerComponent,
      componentName: 'Properties'
    },
    {
      component: HierachyViewerComponent,
      componentName: 'Hierachy'
    }
  ],
  
  defaultLayout: {
    settings:{
      // hasHeaders: true,
    },
    dimensions: {
      minItemHeight: 300,
      minItemWidth: 300,
    },
    content: [
    {
      type: 'row',
      content: [
      {
        type: 'column',
        content: [
        {
          type: 'row',
          content: [
          {//Hierarchy View
            type: 'component',
            width: 20,
            componentName: 'Hierachy',
          },
          {//Editor/Game View
            type: 'stack',
            content: [
            { 
              type: 'component', 
              componentName: 'Scene', 
              componentState: { 
                text: "Editor Window Here" 
              },
              isClosable: false 
            }, 
            { 
              type: 'component', 
              componentName: 'test-panel', 
              componentState: {
                text: 'Game Window Here'
              }, 
              isClosable: false
            }]
          }]
        },
        {//Project File View
          type: 'component',
          height: 20,
          componentName: 'Project',
        }]
      },
      {//Game Component View
        type: 'component',
        width: 20,
        componentName: 'Properties',
        isClosable: false
      }]
    }]
  }
}

@NgModule({
  declarations: [
    AppComponent,
    TestPanelComponent,
    SceneViewerComponent,
    ProjectViewerComponent,
    HierachyViewerComponent,
    PropertiesViewerComponent,
    Vector3Viewer,
    Vector2Viewer,
    MenuBarComponent,
    StringViewer,
    SliderViewer,
    BoolViewer,
    TestPanelComponent,
  ],
  entryComponents: [
    SceneViewerComponent,
    ProjectViewerComponent,
    HierachyViewerComponent,
    PropertiesViewerComponent,
    Vector3Viewer,
    Vector2Viewer,
    SliderViewer,
    StringViewer,
    BoolViewer,
    TestPanelComponent,
    MenuBarComponent,
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    BrowserAnimationsModule,
    CommonModule,
    FormsModule,
    MatToolbarModule,
    MatCheckboxModule,
    MatIconModule,
    MatButtonModule,
    MatMenuModule,
    MatExpansionModule,
    MatSlideToggleModule,
    MatSliderModule,
    GoldenLayoutModule.forRoot(goldenLayoutConfig)
  ],
  providers: [
    // DEFAULT_LOCAL_STORAGE_STATE_STORE_PROVIDER
    PropertiesViewerService,
    PropertyLabels
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
