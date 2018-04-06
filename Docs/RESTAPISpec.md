# REST API v0.0

## Game Properties

| Description | HTTP Verb | Path |
| :---------- | :-------- | :--- |
| List of all available component types | GET | /Components |
| List of all available system types | GET | /Systems |
| List of all loaded worlds | GET | /Worlds |

## Entity Properties

| Description | HTTP Verb | Path |
| :---------- | :-------- | :--- |
| List of all components on an entity | GET | /Worlds/\<**World**>/Entities/\<**ID**>/Components |
| Read component by type | GET | /Worlds/\<**World**>/Entities/\<**ID**>/Components/\<**Type**> |
| ID of the entity | GET | /Worlds/\<**World**>/Entities?Name=\<**Name**>|
| Modify all the data of a component | PUT | /Worlds/\<**World**>/Entities/\<**ID**>/Components/\<**Component**> |
| Modify a single value of a component | PATCH | /Worlds/\<**World**>/Entities/\<**ID**>/Components/\<**Component**>?Field=<**Field**> |
| Change the name of an entity | PATCH | /Worlds/\<**World**>/Entities/\<**ID**> |
| Add a component | PUT | /Worlds/\<**World**>/Entities/\<**ID**>/Components/<**Component**> |
| Remove a component | DELETE | /Worlds/\<**World**>/Entities/\<**ID**>/Components/<**Component**> |
| List of systems applying logic | GET | /Worlds/\<**World**>/Systems?EntityID=\<**ID**> |
