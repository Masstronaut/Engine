import { Component, Input } from '@angular/core';
import { PropertyLabels } from '../propertyLabels';

@Component({
    selector: 'bool',
    templateUrl: './boolViewer.html',
    styleUrls: ['./boolViewer.scss'] 
})

export class BoolViewer {
    name : string;
    slideToggle: string;

    constructor(labels: PropertyLabels) {
        this.name = this.name || labels.name;
        this.slideToggle = "true";
    }

}