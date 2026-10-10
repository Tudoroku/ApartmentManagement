# Apartment Residents Manager

A desktop application for managing apartment residents, built with **C++20**, **Qt 6 Widgets**, and **CMake**. The project focuses on object-oriented design, separating data validation, storage, business logic, and the user interface.

## Features

- **Resident management:** add and delete residents, update an owner's name, and view records in a table.
- **Validation:** reject invalid input and duplicate apartment numbers, with error messages shown in the GUI.
- **Search and organization:** find residents by apartment number in the service layer; filter by apartment type or maximum area; sort by owner name, area, or type followed by area.
- **Undo:** reverse the most recent add, delete, or owner-name update using polymorphic undo actions.
- **Reports:** display the number of apartments for each apartment type.
- **Notification list:** add apartments to a separate list, generate a random selection of distinct apartments, clear the list, and export it to CSV.
- **Synchronized views:** display notifications in a Qt list view or a custom-painted window, both updated through the Observer pattern.
- **File persistence:** automatically load and save resident records through a file-backed repository.

## Tech stack

- **Language:** C++20
- **GUI:** Qt 6 Widgets
- **Build system:** CMake 3.28+
- **Testing:** C++ assertions in a separate `run_tests` executable

## Architecture

The application separates responsibilities into several components:

| Directory | Responsibility |
| --- | --- |
| `domain/` | Resident model (`Locatar`), input validation, and exceptions |
| `repository/` | Abstract repository interface; vector-, map-, and file-backed implementations |
| `service/` | Business operations, notification list, apartment-type reports, and undo actions |
| `gui/` | Qt Widgets interface, notification views, table/list models, and reports |
| `utils/` | Observer/Observable infrastructure and supporting utilities |
| `tests/` | Tests for domain, repositories, services, undo, and file operations |
| `ui/` | Separate console UI source (not included in the Qt executable) |

**Design choices:**

- The service depends on the `RepoLocatari` interface, allowing different repository implementations.
- `UndoAdauga`, `UndoSterge`, and `UndoModifica` implement a common undo interface (Command-style undo).
- Notification views subscribe to changes using **Observer**, keeping multiple windows in sync.
- Data validation and domain-specific exceptions are kept separate from GUI code.

## Build and run

### Prerequisites

- A compiler supporting **C++20**
- **CMake 3.28** or newer
- **Qt 6** with the **Widgets** component

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

If CMake cannot find Qt, configure it with the appropriate Qt installation path, for example:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x/your-compiler-kit"
cmake --build build
```

Start the application (for a single-configuration generator):

```bash
# Linux / macOS
./build/lab6_7_qt

# Windows (PowerShell)
.\build\lab6_7_qt.exe
```

You can also open `CMakeLists.txt` in **CLion** and run the `lab6_7_qt` target with a configured Qt toolchain. With multi-configuration generators, the executable may be inside a `Debug/` or `Release/` subdirectory.

## Using the application

Each resident record contains an **apartment number**, **owner name**, **area**, and **apartment type**. Select a row to populate the input fields, then use the action buttons to add or remove a record, change its owner's name, or undo an operation.

Sorting and filtering affect the displayed table. The **Report** button opens the apartment-type summary. The notification controls let you add an apartment, generate a random list, or open the list and graphical notification windows. The notification-list window also provides **CSV export**.

### Local data

The GUI uses `RepoLocatariFile` with a file named **`memory.txt`**, resolved relative to the application's working directory. If the file is absent, the application starts with an empty collection; changes are saved to disk automatically.

Records use this comma-separated format (no header row):

```text
12,Alex Pop,54.5,2 camere
18,Maria Ionescu,38,garsoniera
```

The exported notification CSV uses the same four fields. The default export filename in the notification window is `notificari.csv`.

## Tests

Build the separate test target:

```bash
cmake --build build --target run_tests
```

Run it:

```bash
# Linux / macOS
./build/run_tests

# Windows (PowerShell)
.\build\run_tests.exe
```

Tests cover validation, repository implementations, error handling, CRUD, sorting/filtering, reports, undo, notification lists, probabilistic operation failures, CSV export, and file persistence.

**Note:** The existing `run_tests` target uses GCC-style `--coverage` flags; MSVC users may need to adjust the test target's compiler/linker options. Tests are invoked directly as an executable (they are not registered with CTest).
