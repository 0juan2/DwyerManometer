cmake_minimum_required(VERSION 3.10)
project(DeteccionManometro)

# Especifica el estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Incluye los directorios de encabezados
include_directories(include)

# Agrega el directorio donde se instalaron los encabezados de CPR
include_directories(C:/msys64/home/place/instalaciones/include)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_PREFIX_PATH "~/cpr_install")
set(SFML_STATIC_LIBRARIES OFF) # Si usas versiones estáticas, cambia esto a ON
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
#directorio de JSON
#add_subdirectory(${CMAKE_SOURCE_DIR}/json)

# Agrega los archivos fuente
add_executable(DeteccionManometro main.cpp)
target_link_libraries(DeteccionManometro PRIVATE ${OpenCV_LIBS} sfml-graphics sfml-window sfml-system)
# Link the JSON library
#add_compile_options("-finput-charset=UTF-8" "-fexec-charset=UTF-8")
#target_link_libraries(P1 PRIVATE nlohmann_json::nlohmann_json)
# Link the CPR library
#target_link_libraries(PrecioChilquinta PRIVATE C:/msys64/home/place/instalaciones/lib/libcpr.dll.a)
# Link the PDF library
# target_link_libraries(PrecioChilquinta PRIVATE ${CMAKE_SOURCE_DIR}/aspose/lib/Aspose.PDF.Cpp_vc14x64.lib)
