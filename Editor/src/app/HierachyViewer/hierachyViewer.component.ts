import { Component, Inject, AfterContentInit } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import { UndoRedoManagerService, UndoRedoAction } from '../../assets/Utils/UndoRedo/UndoRedoManager.service';
import * as GoldenLayout from 'golden-layout';

@Component({
    selector: 'hierachy-viewer',
    template: `
    <div id="hierarchyViewer">
        This is for the Hierachy viewer
    </div>
  `
})

@Inject(GoldenLayoutContainer)
export class HierachyViewerComponent implements GlOnResize, GlOnHide, GlOnShow{
    undoRedoManager : UndoRedoManagerService;
    containerManager : GoldenLayout.Container;

    constructor(@Inject(GoldenLayoutComponentState) private state: any,
                @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {
        this.undoRedoManager = UndoRedoManagerService.getInstance();
        this.containerManager = container;
    }

    public onInput(e: Event): void {
        this.container.extendState({
            value: (<HTMLInputElement>e.target).value
        });

        console.log('Hierachy state saved.');
    }

    public glOnResize(): void {
    }

    public glOnShow(): void {
        console.log('Hierachy Showing!');
    }

    public glOnHide(): void {
        console.log('Hierachy Hiding!');
    }

    public ngOnInit() {
    }
}