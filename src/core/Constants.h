#pragma once

#include <SFML/Graphics.hpp>

// Constants

// Stores the constant values used in the game in one place.
// This makes it easier to change things like window size, colors, and gameplay settings.



namespace Constants
{
    // Window

    inline constexpr unsigned int kWindowWidth = 1280;
    inline constexpr unsigned int kWindowHeight = 720;
    inline constexpr const char* kWindowTitle = "DoTheThing";


    // Player physics (from Player.cpp) 

    inline constexpr float kGravity = 2200.f;
    inline constexpr float kFallGravityMultiplier = 1.4f; // extra gravity while falling
    inline constexpr float kJumpStrength = -1000.f;
    inline constexpr float kPlayerGroundY = 500.f;         // hitbox.y clamp while grounded


    // Player animations

    inline constexpr const char* kPlayerFramePathPrefix = "assets/Images/kosu"; //running
    inline constexpr int kPlayerFrameCount = 21;
    inline constexpr int kPlayerFrameNumberDigits = 4;
    inline constexpr const char* kPlayerIdleTexturePath = "assets/Images/idlem.png"; // dedicated idle pose, not reused from the run cycle


    inline constexpr const char* kPlayerJumpFramePathPrefix = "assets/Images/ziplamam"; //jump
    inline constexpr int kPlayerJumpFrameCount = 22;
    inline constexpr int kPlayerJumpPeakFrameIndex = 6; // 
    inline constexpr int kPlayerJumpGroundContactFrameIndex = 13;

    // The jump animation is selected by the player's height instead of time.
    // This makes the peak frame stay longer naturally, while the fall frames pass faster because falling uses stronger gravity.
    inline constexpr float kPlayerJumpRiseHeight = (kJumpStrength * kJumpStrength) / (2.f * kGravity);


    // After landing, recovery frames play for a short fixed time before returning to idle/run.
    inline constexpr float kPlayerJumpLandingSettleDuration = 0.3f;

    inline constexpr float kPlayerGlobalScale = 0.75f;
    inline constexpr float kPlayerBaselineRatio = 0.809f;
    inline constexpr float kPlayerFrameTime = 0.038f;
    inline constexpr float kPlayerHitboxWidth = 100.f; // collision box width
    inline constexpr float kPlayerHitboxHeight = 120.f; // collision box height
    inline constexpr float kPlayerStartX = 230.f;
    inline constexpr float kPlayerStartY = 300.f;
    inline constexpr float kPlayerSpriteYOffset = 20.f;    // vertical offset between hitbox and drawn sprite
    inline constexpr float kPlayerAutoJumpDistance = 220.f; // how far ahead a rock triggers a jump


    inline constexpr const char* kPlayerPickingFramePathPrefix = "assets/Images/picking"; // picking laundry
    inline constexpr int kPlayerPickingFrameCount = 8;
    inline constexpr float kPlayerPickingFrameTime = 0.02f;
    inline constexpr float kPlayerPickingHoldDuration = 0.15f;

    // StartMenu assets (from StartMenu.cpp) 
    inline constexpr const char* kStartMenuBgIntroPath = "assets/Images/arka3.png";
    inline constexpr const char* kStartMenuBgMenuPath = "assets/Images/arka3.png";
    inline constexpr const char* kStartMenuPlayPath = "assets/Images/Play.png";
    inline constexpr const char* kStartMenuPlayPressedPath = "assets/Images/Playpressed.png";
    inline constexpr const char* kStartMenuTutorialPath = "assets/Images/Guide.png";
    inline constexpr const char* kStartMenuTutorialPressedPath = "assets/Images/Guidepressed.png";
    inline constexpr const char* kStartMenuSettingsPath = "assets/Images/Ellipse_5.png";
    inline constexpr const char* kStartMenuSettingsPressedPath = "assets/Images/Ellipsepressed.png";
    inline constexpr const char* kStartMenuBubblePathPrefix = "assets/Images/bubble_";
    inline constexpr const char* kStartMenuPopSoundPath = "assets/Sounds/bubble_pop.wav";
    // Studio logo shown below the loading bar
    inline constexpr const char* kStartMenuStudioLogoPath = "assets/Images/seray5.png";
    inline constexpr float kStudioLogoTargetWidth = 180.f;
    inline constexpr float kStudioLogoY = 600.f; 

    inline constexpr const char* kUiClickSoundPath = "assets/Sounds/click.wav";
    inline constexpr float kUiClickPitch = 1.6f;
    inline constexpr float kUiClickVolume = 75.f;



    // Story dialogue screen 

    inline constexpr const char* kStoryBackgroundPath = "assets/Images/story.png";
    inline constexpr const char* kStorySkipPath = "assets/Images/skip.png";
    inline constexpr const char* kStorySkipPressedPath = "assets/Images/skippressed.png";
    inline constexpr const char* kStoryNextPath = "assets/Images/ok.png";
    inline constexpr const char* kStoryNextPressedPath = "assets/Images/ok_kapali.png";

    inline constexpr float kStorySkipButtonWidth = 144.f;  //Skip
    inline constexpr float kStoryNextButtonWidth = 144.f;   //Next
    inline const sf::Vector2f kStorySkipButtonPos(170.f, 660.f);
    inline const sf::Vector2f kStoryNextButtonPos(1107.f, 660.f);

    //Story dialogue screen text
    inline const sf::Vector2f kStoryTextPos(230.f, 550.f);
    inline constexpr unsigned int kStoryTextSize = 35;
    inline const sf::Color kStoryTextColor(2, 33, 82); //colour
    inline constexpr float kStoryTypewriterCharsPerSecond = 35.f;
    inline constexpr const char* kStoryTypingSoundPath = "assets/Sounds/typing.mp3";
    inline constexpr float kStoryTypingSoundVolume = 60.f;
    inline constexpr float kStoryTypingSoundStartOffset = 2.43f;
    inline constexpr float kStoryTypingSoundUsableEnd = 8.9f;

    // To Do list popup 
    inline constexpr const char* kTodoPopupTexturePath = "assets/Images/to_do.png";
    inline constexpr float kTodoPopupTargetWidth = 550.f;
    inline const sf::Vector2f kTodoPopupPos(640.f, 360.f);
    inline constexpr float kTodoPopupShowDuration = 1.2f;
    inline constexpr float kTodoPopupDisappearDuration = 0.4f;

    // Guide / tutorial popup (opened from the StartMenu's Guide button)
    inline constexpr const char* kGuidePopupTexturePath = "assets/Images/guide__1_.png";
    inline constexpr float kGuidePopupTargetWidth = 590.f;   // eyeball-tune in game
    inline const sf::Vector2f kGuidePopupPos(640.f, 360.f);  // screen center
    inline const sf::Color kGuideDimColor(18, 28, 58, 140);  // dims the menu behind the popup

    // Popup close (X) button -- shared control, pinned to a popup's top-right corner
    inline constexpr const char* kCloseButtonTexturePath = "assets/Images/carpi.png";
    inline constexpr const char* kCloseButtonPressedTexturePath = "assets/Images/carpi_basilmis.png";
    inline constexpr float kCloseButtonTargetWidth = 56.f;
    inline const sf::Vector2f kCloseButtonInset(30.f, 30.f);  // nudge inward from the corner

    // Settings popup (opened from the Settings button; available on every
    // screen except Story). Sliders control the global music / sfx volumes.
    inline constexpr const char* kSettingsPanelTexturePath = "assets/Images/Group_11.png";
    inline constexpr float kSettingsPanelTargetWidth = 560.f;   // eyeball-tune in game
    inline const sf::Vector2f kSettingsPanelPos(640.f, 360.f);  // screen center
    inline const sf::Color kSettingsDimColor(18, 28, 58, 140);  // dims the screen behind the popup

    // Slider layout, as ratios of the panel so it scales with the image.
    // Adjust these to line the sliders up with the speaker / music-note rows.
    inline constexpr float kSettingsSliderXRatio = 0.42f;      // slider left edge, from panel left
    inline constexpr float kSettingsSliderWidthRatio = 0.42f;  // slider length, as fraction of panel width
    inline constexpr float kSettingsSoundRowYRatio = 0.50f;    // speaker row height, from panel top
    inline constexpr float kSettingsMusicRowYRatio = 0.685f;   // music-note row height, from panel top
    inline constexpr float kSettingsSliderHeight = 18.f;
    inline const sf::Color kSettingsSliderTrack(255, 242, 250);
    inline const sf::Color kSettingsSliderFill(250, 150, 190);
    inline const sf::Color kSettingsSliderKnob(2, 33, 82);

    // Settings opener shown in the corner of the gameplay screens (Playing /
    // Grounding / Guide). Reuses the menu's own settings-button art. On the
    // StartMenu the existing bottom-left settings button is used instead.
    inline const sf::Vector2f kSettingsCornerPos(70.f, 650.f); // center; move if it overlaps HUD
    inline constexpr float kSettingsCornerWidth = 60.f;

    // Countdown timer 
    inline constexpr float kCountdownDurationSeconds = 60.f;
    inline const sf::Vector2f kCountdownPos(35.f, 20.f);
    inline constexpr unsigned int kCountdownTextSize = 36;
    inline const sf::Color kCountdownTextColor(2, 33, 82);

    // Game outcome: win/lose system 
    inline constexpr int kWinScoreThreshold = 200;
    inline constexpr int kStartingLives = 3;

    // Win celebration laundry animation

    inline constexpr const char* kWinAnimationTexturePath = "assets/Images/animasyon_makine.png";
    inline constexpr int kWinAnimationGridCols = 4;
    inline constexpr int kWinAnimationGridRows = 4;
    inline constexpr float kWinAnimationFrameTime = 0.08f;
    inline constexpr float kWinAnimationGrowDuration = 0.8f;
    inline constexpr float kWinAnimationHoldDuration = 1.2f;
    inline constexpr float kWinAnimationShrinkDuration = 0.4f;
    inline constexpr float kWinAnimationScale = 0.5f;
    inline const sf::Vector2f kWinAnimationPos(640.f, 360.f);

    // Popup panel (shared look for the Win and Lose popups)
    inline const sf::Color kPopupPanelColor(255, 250, 240);
    inline const sf::Color kPopupPanelOutline(2, 33, 82);
    inline constexpr float kPopupPanelWidth = 520.f;
    inline constexpr float kPopupPanelHeight = 320.f;
    inline const sf::Vector2f kPopupPanelPos(640.f, 360.f);
    inline constexpr unsigned int kPopupTitleSize = 46;
    inline const sf::Color kPopupTitleColor(2, 33, 82);
    inline constexpr float kPopupTitleYOffset = -80.f;

    // Outcome popups: image panels (win / lose) + Menu / Replay image buttons.
    inline constexpr const char* kWinPanelTexturePath = "assets/Images/win.png";
    inline constexpr const char* kLosePanelTexturePath = "assets/Images/lost.png";
    inline constexpr float kOutcomePanelTargetWidth = 540.f; // eyeball-tune in game

    inline constexpr const char* kOutcomeMenuTexturePath = "assets/Images/menu.png";
    inline constexpr const char* kOutcomeMenuPressedTexturePath = "assets/Images/menu_pressed.png";
    inline constexpr const char* kOutcomeReplayTexturePath = "assets/Images/replay.png";
    inline constexpr const char* kOutcomeReplayPressedTexturePath = "assets/Images/replay_pressed.png";
    inline constexpr float kOutcomeButtonWidth = 180.f;
    inline constexpr float kOutcomeButtonY = 600.f;        // absolute Y for both buttons (tunable)
    inline constexpr float kOutcomeButtonXSpacing = 110.f; // each button's offset from panel center X

    inline constexpr float kPopupButtonWidth = 170.f;
    inline constexpr float kPopupButtonHeight = 64.f;
    inline constexpr float kPopupButtonYOffset = 60.f;   // relative to panel center
    inline constexpr float kPopupButtonXSpacing = 100.f; // for the two-button (Menu/Resume) layout
    inline constexpr unsigned int kPopupButtonTextSize = 26;
    inline const sf::Color kPopupButtonFill(70, 170, 110);
    inline const sf::Color kPopupButtonOutline(40, 110, 70);

    // Hearts (lives display)
    inline constexpr const char* kHeartFullTexturePath = "assets/Images/heart.png";
    inline constexpr const char* kHeartLostTexturePath = "assets/Images/lossheart.png";
    inline constexpr float kHeartTargetWidth = 40.f;
    inline constexpr float kHeartSpacing = 46.f;
    inline const sf::Vector2f kHeartsPos(35.f, 70.f);

    // Hyperfocus bar (from Hyperfocus.cpp) 
    inline constexpr float kHyperfocusDuration = 3.5f;   // seconds hyperfocus stays active once triggered
    inline constexpr float kHyperfocusBarWidth = 180.f;
    inline constexpr float kHyperfocusBarHeight = 16.f;
    inline constexpr int kHyperfocusMaxProgress = 100;
    inline const sf::Vector2f kHyperfocusLabelPos(1060.f, 80.f);
    inline const sf::Vector2f kHyperfocusBarPos(1060.f, 130.f);
    inline const sf::Color kHyperfocusLabelColor(237, 101, 163);
    inline const sf::Color kHyperfocusBarBackground(200, 200, 200);

    // Grounding breathing exercise 

    inline constexpr float kGroundingPhaseDuration = 4.f; // seconds per breathing phase

    // Two backgrounds: "Hold Space" (Inhale/HoldIn/Exhale) vs "Release Space" (HoldOut/Wait)
    inline constexpr const char* kGroundingHoldBackgroundPath = "assets/Images/grounding_one.png";
    inline constexpr const char* kGroundingReleaseBackgroundPath = "assets/Images/grounding_two.png";

    // Phase label images 
    inline constexpr const char* kGroundingInhaleLabelPath = "assets/Images/INHALE_transparent.png";
    inline constexpr const char* kGroundingHoldLabelPath = "assets/Images/hold_transparent.png";
    inline constexpr const char* kGroundingExhaleLabelPath = "assets/Images/exhale_transparent.png";

    // Box measured directly from the real background image 
    inline const sf::Vector2f kGroundingBoxPos(447.f, 86.f);
    inline constexpr float kGroundingBoxSize = 384.f;
    inline constexpr float kGroundingBoxInset = 20.f;
    inline constexpr float kGroundingBoxBorderThickness = 18.f;
    inline constexpr float kGroundingBoxCornerRadius = 8.f;
    inline const sf::Color kGroundingBoxProgressColor(250, 150, 190);

    inline const sf::Vector2f kGroundingLabelPos(639.f, 278.f);
    inline constexpr float kGroundingLabelMinScale = 0.13f;
    inline constexpr float kGroundingLabelMaxScale = 0.19f;

    // Distraction popups (from Distraction.cpp) 
    inline constexpr int kDistractionChaosThreshold = 8;   // popups until chaos mode kicks in
    inline constexpr int kDistractionMaxPopups = 25;       // popups until the Grounding button appears
    inline constexpr float kDistractionStartDelay = 5.f;   // seconds before the first popup
    inline constexpr const char* kDistractionPopupTexturePath = "assets/Images/distraction_box.png";

    // Grounding button image (shown once popups pile up; click -> Grounding)
    inline constexpr const char* kGroundingButtonNormalTexturePath = "assets/Images/groundingnew1.png";
    inline constexpr const char* kGroundingButtonPressedTexturePath = "assets/Images/groundingnew2.png";


    // Shared UI colors 
    inline const sf::Color kGroundingButtonFill(70, 170, 110);
    inline const sf::Color kGroundingButtonOutline(40, 110, 70);
    inline const sf::Color kProgressBarTrack(200, 215, 225);
    inline const sf::Color kProgressBarTrackOutline(120, 160, 190);
    inline const sf::Color kProgressBarFill(70, 150, 220);
    inline const sf::Color kHyperfocusBarFill(116, 85, 115);

    // Font (from Game.cpp) 
    inline constexpr const char* kFontPath = "assets/Fonts/LilitaOne-Regular.ttf";

    // Per-screen background clear colors (from Game.cpp's render()/renderGroundingPage()) 
    inline const sf::Color kStartMenuBackgroundColor(254, 245, 231);
    inline const sf::Color kGroundingBackgroundColor(220, 240, 230);
    inline const sf::Color kPlayingBackgroundColor(240, 247, 251);

    // Score HUD (from Game.cpp) 
    inline const sf::Vector2f kScoreTextPos(1060.f, 35.f);
    inline const sf::Color kScoreTextColor(237, 101, 163);
    inline constexpr unsigned int kScoreTextSize = 40;
    inline constexpr const char* kCoinTexturePath = "assets/Images/Coin.png";
    inline constexpr float kCoinScale = 0.35f;
    inline const sf::Vector2f kCoinPos(1020.f, 45.f);

    // World scrolling (from Game.cpp)
    inline constexpr float kWorldSpeed = 1000.f;
    inline constexpr float kHyperfocusSpeedMultiplier = 3.5f;
    inline constexpr float kFloorBoundaryY = 504.f; // wall/floor split line
    inline constexpr float kOffscreenRemovalX = -40.f; // generic despawn threshold for foreground sprites

    // Dirty laundry / rocks gameplay tuning (from Game.cpp) 
    inline constexpr float kAutoCollectRadius = 150.f; // hyperfocus auto-pickup radius for laundry
    inline constexpr int kLaundryGainNormal = 10;
    inline constexpr int kLaundryGainHyperfocus = 20;
    inline constexpr int kRockPenalty = 5;

    // Coin-burst collection feedback (plays at the collected item's position) 
    inline constexpr const char* kCoinBurstTexturePath = "assets/Images/coinanimation.png";
    inline constexpr int kCoinBurstGridCols = 4;
    inline constexpr int kCoinBurstGridRows = 3;
    inline constexpr float kCoinBurstFrameTime = 0.05f;
    inline constexpr float kCoinBurstScale = 0.5f;
    inline constexpr float kCoinBurstYOffset = -90.f;

    inline constexpr const char* kCoinSoundPath = "assets/Sounds/coin.mp3";
    inline constexpr int kCoinSoundVoiceCount = 4;
    inline constexpr float kCoinSoundVolume = 40.f;
    inline constexpr float kCoinSoundPitch = 1.f;

    // Rock/puddle hit error sound (plays when the player hits a hazard)
    inline constexpr const char* kErrorSoundPath = "assets/Sounds/error.mp3";
    inline constexpr int kErrorSoundVoiceCount = 4;
    inline constexpr float kErrorSoundVolume = 60.f;
    inline constexpr float kErrorSoundPitch = 1.f;

    // Background music (loops the whole game; muted only during Grounding)
    inline constexpr const char* kGameMusicPath = "assets/Sounds/music.mp3";
    inline constexpr float kGameMusicVolume = 35.f;

    // Settings "Okay" button (replaces the X on the settings popup): same
    // width as the Story skip/next buttons, centered along the panel's bottom.
    inline constexpr const char* kSettingsOkayTexturePath = "assets/Images/okay.png";
    inline constexpr const char* kSettingsOkayPressedTexturePath = "assets/Images/Okaz.png";
    inline constexpr float kSettingsOkayWidth = 144.f;      // matches kStorySkipButtonWidth
    inline constexpr float kSettingsOkayYRatio = 0.86f;     // vertical spot on the panel (tunable)

    // World asset paths (from Game.cpp)
    inline constexpr const char* kWallTexturePath = "assets/Images/duvar4.png";
    inline constexpr const char* kFloorTexturePath = "assets/Images/morzemin.png";
    inline constexpr const char* kShelfTexture1Path = "assets/Images/transparent_output.png";
    inline constexpr const char* kShelfTexture2Path = "assets/Images/transparent_output2.png";
    inline constexpr const char* kMachineTexturePath = "assets/Images/washmash.png";
    inline constexpr const char* kDirtyLaundryTexturePath = "assets/Images/dirty_laundry.png";
    inline constexpr const char* kPuddleTexturePath = "assets/Images/water.png";

    // --- Background decorations 
    inline constexpr const char* kCabinetTexturePath = "assets/Images/cabinet.png";
    inline constexpr const char* kWindowTexturePath = "assets/Images/window.png";
}