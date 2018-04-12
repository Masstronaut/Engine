import { Component, Input, Inject } from '@angular/core';
import { PropertyLabels } from '../propertyLabels';

@Component({
    selector: 'vec3',
    templateUrl: './vec3Viewer.html',
    styleUrls: ['./vec3Viewer.scss'] 
})

export class Vector3Viewer {
    name : string;
    xLabel: string;
    yLabel: string;
    zLabel: string;

    constructor(labels: PropertyLabels) {
        this.name = this.name || labels.name;
        this.xLabel = this.xLabel || labels.xLabel;
        this.yLabel = this.yLabel || labels.yLabel;
        this.zLabel = this.zLabel || labels.zLabel;
    }

}