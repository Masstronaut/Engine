import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { of } from 'rxjs/observable/of';
import { catchError, map, tap } from 'rxjs/operators';

@Injectable()
export class PropertiesViewerService {
    private engineUrl = '../../assets';

    constructor(
        private http: HttpClient) {}

    getComponents() {
        const url = `${this.engineUrl}/components.json`;
        return this.http.get(url).pipe(
            tap(component => console.log('fetching all components')),
            catchError(this.handleError('getComponents', []))
        );
    }

    getGameObjectComponents() {
        const url = `${this.engineUrl}/propertiesData.json`;

        return this.http.get(url).pipe(
            tap(component => console.log('fetching gameobject components')),
            catchError(this.handleError('getGameObjectComponents', []))
        );
    }

    private handleError<T> (operation = 'operation', result?: T) {
        return (error: any): Observable<T> => {
            
            //TODO: find a better way to display/log errors
            console.error(error);

            // Let the app keep running by returning an empty result.
            return of(result as T);
        };
    }
}