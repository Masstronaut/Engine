import { Component, Input } from '@angular/core';

export class ContextMenu {
    x: number;
    y: number;
    show: boolean;

    constructor() {
        this.x = this.y = 0;
        this.show = false;
    }
}

@Component({
    selector: 'context-menu',
    templateUrl: './contextMenu.html',
    styleUrls: ['./contextMenu.scss']
})

export class ContextMenuComponent {
    @Input()
    x;
    @Input()
    y;

    constructor() {
        this.x = this.y = 0;
    }
    
};