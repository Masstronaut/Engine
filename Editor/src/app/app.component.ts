import { Component, OnInit } from '@angular/core';
import { MenuBarComponent } from './MenuBar/menuBar.component';

@Component({
  selector: 'editor',
  template: `
  <div class="editorContainer">
    <menu-bar></menu-bar>
    <golden-layout-root></golden-layout-root>
  </div>
  `,
  //templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  //title = 'app';
}
