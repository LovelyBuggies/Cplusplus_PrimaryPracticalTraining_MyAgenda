# Cplusplus_PrimaryPracticalTraining_MyAgenda

> Practical Training for freshmen of SDCS, SYSU. 
> Directed by Maolin Pan, SDCS, SYSU. 
> Author Nino Lau.

## Introduce

This project provides implementations of MyAgenda, which serves as an agenda for user. Usage and Techs shown in *./document/Feelings.pdf* and *./test/test.pdf*.

## Architecture

MyAgenda architecture can be represented by three layers: **Data Layer**, **Logic Layer**, **UI Layer**.
* UI Layer -- AgendaUI class: Responsible for user interaction, passing the operation to Logic Layer.
* Logic Layer -- AgendaService class: Dealing with the operation from user, passing the operation to Data Layer and change the values.
* Data Layer -- Storage class, Date class, User class and Meeting class: Underlying part, Storage Class provides the operational interface for Date class, User class and Meeting class. Only one instance is allowed at a time to manipulate the underlying data.

<!--UML Class Diagram Later!!!-->

## Folder Structure

.
+-- code
|   +-- doc
|   |   +-- Agenda.cpp
|   |   +-- AgendaService.cpp
|   |   +-- AgendaUI.cpp
|   |   +-- Date.cpp
|   |   +-- History.cpp
|   |   +-- Meeting.cpp
|   |   +-- Storage.cpp
|   |   +-- User.cpp
|   +-- src
|   |   +-- Agenda.hpp
|   |   +-- AgendaService.hpp
|   |   +-- AgendaUI.hpp
|   |   +-- Date.hpp
|   |   +-- History.hpp
|   |   +-- Meeting.hpp
|   |   +-- Storage.hpp
|   |   +-- User.hpp
+-- document
|   +-- Feelings.pdf
+-- test
|   +-- a.out
|   +-- test.pdf
+-- README.md

## Usage

Usage and Techs shown in *./document/Feelings.pdf* and *./test/test.pdf*.

