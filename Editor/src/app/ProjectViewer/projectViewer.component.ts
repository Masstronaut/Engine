import { Component, Inject } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import * as GoldenLayout from 'golden-layout';

@Component({
  selector: 'project-viewer',
  template: `
    <div>
      This is for the project viewer
    </div>
  `
})
export class ProjectViewerComponent implements GlOnResize, GlOnHide, GlOnShow {

  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {}

  public onInput(e: Event): void {
    
    this.container.extendState({
      value: (<HTMLInputElement>e.target).value
    });

    console.log('Project state saved.');
  }

  public glOnResize(): void {
    console.log('Project Resizing!');
  }

  public glOnShow(): void {
    console.log('Project Showing!');
  }

  public glOnHide(): void {
    console.log('Project Hiding!');
  }

  /*public glOnTab(): void {
    console.log('Tab shown!');
  }*/
}