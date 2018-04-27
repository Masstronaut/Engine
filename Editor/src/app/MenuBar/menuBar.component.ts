import { Component, OnInit, ViewChild } from '@angular/core';
import { MatMenuTrigger } from '@angular/material/menu';
 
@Component({
  selector: 'menu-bar',
  templateUrl: './menuBar.html',
  styleUrls: ['./menuBar.css'],
})
export class MenuBarComponent {
    // File dropdown options:
  gFileOptions: Array<Object>;
  gEditOptions: Array<Object>;

    // File Options for Menu Bar:
  MenuButton_NewProjectTrigger() {
    console.log("New Project Menu Button!");
  }

  MenuButton_OpenProjectTrigger() {
    console.log("Open Project Menu Button!");
  }

  MenuButton_SaveProjectTrigger() {
    console.log("Save Project Menu Button!");
  }

    // Edit Options for Menu Bar
  MenuButton_CutTrigger() {
    console.log("Cut Menu Button!");
  }

  MenuButton_CopyTrigger() {
    console.log("Copy Menu Button!");
  }

  MenuButton_PasteTrigger() {
    console.log("Paste Menu Button!");
  }

  MenuButton_DuplicateTrigger() {
    console.log("Duplicate Menu Button!");
  }

  MenuButton_DeleteTrigger() {
    console.log("Delete Menu Button!");
  }


  constructor() {
    this.gFileOptions = [
      {
        optionName: "New Project", 
        onclickTrigger: this.MenuButton_NewProjectTrigger
      },
      {
        optionName: "Open Project",
        onclickTrigger: this.MenuButton_OpenProjectTrigger
      },
      {
        optionName: "Save Project",
        onclickTrigger: this.MenuButton_SaveProjectTrigger
      }
    ];

      // set up the Edit Function Menu, sets up the name for each button as well as the triggers
    this.gEditOptions = [
      {
        optionName: "Cut", 
        onclickTrigger: this.MenuButton_CutTrigger
      },
      {
        optionName: "Copy",
        onclickTrigger: this.MenuButton_CopyTrigger
      },
      {
        optionName: "Paste",
        onclickTrigger: this.MenuButton_PasteTrigger
      },
      {
        optionName: "Duplicate",
        onclickTrigger: this.MenuButton_DuplicateTrigger
      },
      {
        optionName: "Delete",
        onclickTrigger: this.MenuButton_DeleteTrigger
      }
    ];
   }
}
