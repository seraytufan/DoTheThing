cmake_minimum_required(VERSION 3.16)

project(DoTheThing)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Libraries/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system audio network REQUIRED)

add_executable(DoTheThing
    src/main.cpp

    # core/ 
    src/core/Game.cpp
    

    # entities/ 
    src/entities/Player.cpp

    # mechanics/ 
    src/mechanics/Hyperfocus.cpp
    src/mechanics/Distraction.cpp
    src/mechanics/Grounding.cpp

    #  ui/
    src/ui/TextHelper.cpp
    src/ui/ProgressBar.cpp
    src/ui/Button.cpp
    src/ui/TypewriterText.cpp
    src/ui/CountdownTimer.cpp
    src/ui/HeartDisplay.cpp
    src/ui/ShapeButton.cpp
    src/ui/SpritesheetPopEffect.cpp
    src/ui/BoxTraceProgress.cpp
    src/ui/CoinBurstEffect.cpp
    src/ui/CloseButton.cpp
    src/ui/Slider.cpp

    #  audio/ 
    src/audio/SoundPool.cpp

    #  world/ 
    src/world/ParallaxLayer.cpp
    src/world/ObjectSpawner.cpp
    src/world/LaundryRoomBackground.cpp

    # states/
    src/states/StartMenuState.cpp
    src/states/StoryState.cpp
    src/states/TodoPopupState.cpp
    src/states/PlayingState.cpp
    src/states/GroundingState.cpp
    src/states/GuideState.cpp
    src/states/SettingsState.cpp

    
)

target_include_directories(DoTheThing PRIVATE src)

target_link_libraries(DoTheThing
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)

add_custom_command(TARGET DoTheThing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:DoTheThing>/assets"
)