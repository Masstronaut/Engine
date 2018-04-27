import { Component, Inject } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import * as GoldenLayout from 'golden-layout';

@Component({
  selector: 'hierachy-viewer',
  template: `
    <div>
      This is for the Hierachy viewer
    </div>
  `
})
export class HierachyViewerComponent implements GlOnResize, GlOnHide, GlOnShow {

  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {}

  public onInput(e: Event): void {
    
    this.container.extendState({
      value: (<HTMLInputElement>e.target).value
    });

    console.log('Hierachy state saved.');
  }

  public glOnResize(): void {
    console.log('Hierachy Resizing!');
  }

  public glOnShow(): void {
    console.log('Hierachy Showing!');
  }

  public glOnHide(): void {
    console.log('Hierachy Hiding!');
  }

  /*public glOnTab(): void {
    console.log('Tab shown!');
  }*/
}