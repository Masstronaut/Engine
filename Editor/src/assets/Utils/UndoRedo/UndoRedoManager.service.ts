import {Injectable} from '@angular/core';
//import { exec } from 'child_process';

interface UndoInterface {
    Execute();
    UnExecute();
}

export class UndoRedoAction implements UndoInterface {
    mExecuteTask : Function;
    mUnexecuteTask : Function;
    mExecuteTaskParams : Array<any>;
    mUnexecuteTaskParams : Array<any>;

    constructor() {
    }

    public setExecuteFunc(executeTask : Function, ...args: any[]) {
        this.mExecuteTask = executeTask;
        this.mExecuteTaskParams = args;
    }

    public setUnexecuteFunc(unexecuteTask : Function, ...args: any[]) {
        this.mUnexecuteTask = unexecuteTask;
        this.mUnexecuteTaskParams = args;
    }

    public Execute() {
        if(this.mExecuteTaskParams.length != 0) {
            this.mExecuteTask(this.mExecuteTaskParams);
        }
        else {
            this.mExecuteTask();
        }
    }

    public UnExecute() {
        if(this.mUnexecuteTaskParams.length != 0) {
            this.mUnexecuteTask(this.mUnexecuteTaskParams);
        }
        else {
            this.mUnexecuteTask();
        }
    }
}


@Injectable()
export class UndoRedoManagerService {
    private static instance: UndoRedoManagerService;
    private redoStack : Array<UndoRedoAction>;
    private undoStack : Array<UndoRedoAction>;

    constructor() {
        this.redoStack = Array<UndoRedoAction>();
        this.undoStack = Array<UndoRedoAction>();
    }

    static getInstance() {
        if (!UndoRedoManagerService.instance) {
            UndoRedoManagerService.instance = new UndoRedoManagerService();
        }
        return UndoRedoManagerService.instance;
    }

    public Undo() {
        if(this.undoStack.length != 0) {
            var undoTask = this.undoStack[this.undoStack.length - 1];
            undoTask.UnExecute();
            this.undoStack.pop();
            this.redoStack.push(undoTask);
        }
    }
    public Redo() {
        if(this.redoStack.length != 0) {
            var redoTask = this.redoStack[this.redoStack.length - 1];
            redoTask.Execute();
            this.redoStack.pop();
            this.undoStack.push(redoTask);
        }
    }

    public Add(action : UndoRedoAction) {
        this.undoStack.push(action);
    }
};