import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { of } from 'rxjs/observable/of';
import { catchError, map, tap } from 'rxjs/operators';

@Injectable()
export class PropertiesViewerService {
    private engineUrl = '../../assets';
    private realEngineURL = 'http://localhost:42069/api'

    constructor(private http: HttpClient) {}

    getComponentsList() {
        const url = `${this.realEngineURL}/ComponentsList`;
        return this.http.get(url);
    }

    getComponentsStructures() {
        const url = `${this.realEngineURL}/Components`;
        return this.http.get(url);
    }

    getComponents() {
        const url = `${this.engineUrl}/components.json`;
        return this.http.get(url);
    }

    getGameObjectComponents() {
        const url = `${this.engineUrl}/propertiesData.json`;

        return this.http.get(url);
    }
}