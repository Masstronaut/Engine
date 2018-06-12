import { Component, Inject, Injector, ComponentRef, ReflectiveInjector } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import { PropertiesViewerService } from './propertiesViewer.service';
import { PropertyLabels } from './Properties/propertyLabels';
import { Vector3Viewer } from './Properties/vec3Viewer/vec3Viewer.component';
import { Vector2Viewer } from './Properties/vec2Viewer/vec2Viewer.component';
import { BoolViewer } from './Properties/boolViewer/boolViewer.component';
import { StringViewer } from './Properties/stringViewer/stringViewer.component';
import { SliderViewer } from './Properties/sliderViewer/sliderViewer.component';
import { PropertiesViewerVMService, property } from './VMLayer/propertiesViewerVM.service';
import * as GoldenLayout from 'golden-layout';


@Component({
    selector: 'properties-viewer',
    templateUrl: './propertiesViewer.html',
    styleUrls: ['./propertiesViewer.css']    
})

export class PropertiesViewerComponent implements GlOnResize, GlOnHide, GlOnShow {
    public static containerManager : GoldenLayout.Container;

    constructor(@Inject(GoldenLayoutComponentState) private state: any,
                @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container,
                private propertiesViewerVMService: PropertiesViewerVMService) {
    
        PropertiesViewerComponent.containerManager = container;
    }

    getComponentsList() : Array<String> {
        return this.propertiesViewerVMService.getComponentsList();
    }

    getGameObjectProperties() : Array<property> {
        return this.propertiesViewerVMService.getGameObjectProperties();
    }

    addComponent(componentName: string) {
        var componentProperty = this.propertiesViewerVMService.addComponent(componentName);
    }
    
    removeComponent(componentName:string) {
        this.propertiesViewerVMService.removeComponent(componentName);
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

    public ngOnInit() {
    }

    public ngOnDestroy() {
    }
}