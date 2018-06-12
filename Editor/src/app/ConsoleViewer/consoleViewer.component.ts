import { Component, Inject, ComponentRef, Injectable } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import * as GoldenLayout from 'golden-layout';


@Component({
    selector: 'console-viewer',
    templateUrl: './consoleViewer.html',
    styleUrls: ['./consoleViewer.scss']    
})


export class ConsoleViewerComponent implements GlOnResize, GlOnHide, GlOnShow {
    public static containerManager: GoldenLayout.Container;

    constructor(@Inject(GoldenLayoutComponentState) private state: any,
                @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {
        ConsoleViewerComponent.containerManager = container;
    }

    ngOnInit() {
    }

    public onInput(e: Event): void {
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

    public ngOnDestroy() {
        var stop = 0;
    }
}