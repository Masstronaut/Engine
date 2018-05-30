import { Component, Input } from '@angular/core';
import { PropertyLabels } from '../propertyLabels';

@Component({
    selector: 'string',
    templateUrl: './stringViewer.html',
    styleUrls: ['./stringViewer.scss'] 
})

export class StringViewer {
    @Input()
    name : string;

    constructor(labels: PropertyLabels) {
        this.name = this.name || labels.name;
    }

}