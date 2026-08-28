cmake_minimum_required(VERSION 3.16)

project(JobApplicationsTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    src/main.cpp
    src/models/JobApplication.cpp
    src/models/InterviewQuestion.cpp
    src/database/DatabaseManager.cpp
    src/repositories/ApplicationRepository.cpp
    src/repositories/QuestionRepository.cpp
    src/ui/MainWindow.cpp
    src/ui/ApplicationDialog.cpp
    src/ui/ApplicationDetailsDialog.cpp
    src/ui/QuestionDialog.cpp
    src/ui/MockInterviewDialog.cpp
)

set(PROJECT_HEADERS
    src/models/JobApplication.h
    src/models/InterviewQuestion.h
    src/database/DatabaseManager.h
    src/repositories/ApplicationRepository.h
    src/repositories/QuestionRepository.h
    src/ui/MainWindow.h
    src/ui/ApplicationDialog.h
    src/ui/ApplicationDetailsDialog.h
    src/ui/QuestionDialog.h
    src/ui/MockInterviewDialog.h
)

add_executable(JobApplicationsTracker
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(JobApplicationsTracker PRIVATE src)

target_link_libraries(JobApplicationsTracker PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

add_custom_command(TARGET JobApplicationsTracker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/database
            $<TARGET_FILE_DIR:JobApplicationsTracker>/database
)
