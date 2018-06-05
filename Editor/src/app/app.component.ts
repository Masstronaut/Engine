import { Component, OnInit } from '@angular/core';
import { MenuBarComponent } from './MenuBar/menuBar.component';
import { UndoRedoManagerService } from '../assets/Utils/UndoRedo/UndoRedoManager.service';

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
  undoRedoManager : UndoRedoManagerService;
  constructor() {
    this.undoRedoManager = UndoRedoManagerService.getInstance();
  }

  //title = 'app';
  ngOnInit() {
    this.LoadEventListeners();
  }


  LoadEventListeners() {
    document.addEventListener('keydown', (event) => {
            const keyName = event.key;
            if(keyName === 'Control') {
                return;
            }

            if(event.ctrlKey) {
                  // undo, Ctrl+Z
                if(event.keyCode == 90) {
                  this.undoRedoManager.Undo();
                  return;
                }

                  // redo, Ctrl+Y
                if(event.keyCode == 89) {
                  this.undoRedoManager.Redo();
                  return;
                }
            }
        }
    )
}
}

