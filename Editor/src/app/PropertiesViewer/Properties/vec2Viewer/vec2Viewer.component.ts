import { Component, Input } from '@angular/core';
import { PropertyLabels } from '../propertyLabels';

@Component({
    selector: 'vec2',
    templateUrl: './vec2Viewer.html',
    styleUrls: ['./vec2Viewer.scss'] 
})

export class Vector2Viewer {
    name : string;
    xLabel: string;
    yLabel: string;

    constructor(labels: PropertyLabels) {
        this.name = this.name || labels.name;
        this.xLabel = this.xLabel || labels.xLabel;
        this.yLabel = this.yLabel || labels.yLabel;
    }

}