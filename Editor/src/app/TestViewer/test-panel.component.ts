import { Component, Inject } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import { ContextMenu, ContextMenuComponent } from '../ContextMenu/contextMenu.component';
import * as GoldenLayout from 'golden-layout';

@Component({
  selector: 'test-panel',
  templateUrl: 'test-panel.html'
})
export class TestPanelComponent implements GlOnResize, GlOnHide, GlOnShow {
  rightClickMenu: ContextMenu;

  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {

    this.rightClickMenu = new ContextMenu();
  }

  onRightClick(e: MouseEvent): void {
    
    this.rightClickMenu.x = e.offsetX;
    this.rightClickMenu.y = e.offsetY;
    this.rightClickMenu.show = true;
  }

  disableRightClickMenu(): void {
    this.rightClickMenu.show = false;
  }

  public onInput(e: Event): void {
    
    this.container.extendState({
      value: (<HTMLInputElement>e.target).value
    });

    console.log('state saved.');
  }

  public glOnResize(): void {
    console.log('Resizing!');
  }

  public glOnShow(): void {
    console.log('Showing!');
  }

  public glOnHide(): void {
    console.log('Hiding!');
  }

  /*public glOnTab(): void {
    console.log('Tab shown!');
  }*/
}