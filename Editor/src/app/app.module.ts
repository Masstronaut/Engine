import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

//Angular Material
import { MatMenuModule } from '@angular/material/menu';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatButtonModule } from '@angular/material/button';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatSlideToggleModule } from '@angular/material/slide-toggle';
import { MatExpansionPanel, MatExpansionModule } from '@angular/material/expansion';
import { MatSliderModule } from '@angular/material/slider';
import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';

//Custom Components/Servies
import { ContextMenuComponent } from './ContextMenu/contextMenu.component'
import { MenuBarComponent } from './MenuBar/menuBar.component';
import { TestPanelComponent } from './TestViewer/test-panel.component';
import { SceneViewerComponent } from './SceneViewer/sceneViewer.component';
import { ProjectViewerComponent } from './ProjectViewer/projectViewer.component';
import { HierachyViewerComponent } from './HierachyViewer/hierachyViewer.component';
import { PropertiesViewerComponent } from  './PropertiesViewer/propertiesViewer.component';
import { PropertiesViewerService } from './PropertiesViewer/propertiesViewer.service';
import { PropertiesViewerVMService } from './PropertiesViewer/VMLayer/propertiesViewerVM.service';
import { UndoRedoManagerService } from '../assets/Utils/UndoRedo/UndoRedoManager.service';
import { PropertyLabels } from './PropertiesViewer/Properties/propertyLabels';
import { StringViewer } from './PropertiesViewer/Properties/stringViewer/stringViewer.component';
import { SliderViewer } from './PropertiesViewer/Properties/sliderViewer/sliderViewer.component';
import { Vector3Viewer } from './PropertiesViewer/Properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from './PropertiesViewer/Properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from './PropertiesViewer/Properties/boolViewer/boolViewer.component';
import { ConsoleViewerComponent } from './ConsoleViewer/consoleViewer.component';

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
      componentName: 'Hierarchy'
    },
    {
      component: ConsoleViewerComponent,
      componentName: 'Console'
    }
  ],

  defaultLayout: {
    settings: {
      // hasHeaders: false
    },
    dimensions: {
      minItemHeight: 200,
      minItemWidth: 200,
      borderWidth: 3
    },
    content: [
    {
      type: 'column',
      content: [
      {
        type: 'row',
        content: [
        {
          type: 'row',
          content: [
          {//Hierarchy View
            type: 'component',
            width: 20,
            height: 20,
            componentName: 'Hierarchy',
          },
          {//Editor/Game View
            type: 'stack',
            content: [
            { 
              type: 'component',
              width: 20,
              height: 20,
              componentName: 'Scene', 
              componentState: { 
                text: "Editor Window Here" 
              },
            }, 
            { 
              type: 'component', 
              width: 20,
              height: 20,
              componentName: 'test-panel', 
              componentState: {
                text: 'Game Window Here'
              }, 
              isClosable: false
            }]
          },
          {//Game Component View
            type: 'component',
            width: 20,
            height: 20,
            componentName: 'Properties',
          }]
        }]
      },
      {//Project File View
        type: 'component',
        width: 20,
        height: 20,
        componentName: 'Project',
      },      
      {//Project File View
        type: 'component',
        width: 0,
        height: 0,
        componentName: 'Console',
      },
    ]
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
    ConsoleViewerComponent,
    MenuBarComponent,
    ContextMenuComponent,
    Vector3Viewer,
    Vector2Viewer,
    StringViewer,
    SliderViewer,
    BoolViewer,
    TestPanelComponent,
  ],
  entryComponents: [
    TestPanelComponent,
    SceneViewerComponent,
    ProjectViewerComponent,
    HierachyViewerComponent,
    MenuBarComponent,
    PropertiesViewerComponent,
    ConsoleViewerComponent,
    Vector3Viewer,
    Vector2Viewer,
    SliderViewer,
    StringViewer,
    BoolViewer,
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
    MatListModule,
    GoldenLayoutModule.forRoot(goldenLayoutConfig)
  ],
  providers: [
    // DEFAULT_LOCAL_STORAGE_STATE_STORE_PROVIDER
    PropertiesViewerService,
    PropertiesViewerVMService,
    UndoRedoManagerService,
    PropertyLabels
  ],
  bootstrap: [AppComponent]
})

export class AppModule { 
}
