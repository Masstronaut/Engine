import { Component, Input } from '@angular/core';
import { PropertyLabels } from '../propertyLabels';

@Component({
    selector: 'slider',
    templateUrl: './sliderViewer.html', 
    styleUrls: ['./sliderViewer.scss'] 
})

export class SliderViewer {
    name : string;
    min : number;
    max : number;
    isSlider : string;
    value : number;
    constructor(labels: PropertyLabels) {
        this.name = this.name || labels.name;
        this.isSlider = "true";
        this.value = 0;
    }

}