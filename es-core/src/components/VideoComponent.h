#ifndef _VIDEOCOMPONENT_H_
#define _VIDEOCOMPONENT_H_

#include "platform.h"
#include GLHEADER

#include "GuiComponent.h"
#include "ImageComponent.h"
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_mutex.h>
#include <boost/filesystem.hpp>

class VideoComponent : public GuiComponent
{
	// Structure that groups together the configuration of the video component
	struct Configuration
	{
		unsigned						startDelay;
		bool							showSnapshotNoVideo;
		bool							showSnapshotDelay;
		std::string						defaultVideoPath;
	};

public:
	VideoComponent(Window* window);
	virtual ~VideoComponent();

	// Loads the video at the given filepath
	bool setVideo(std::string path);
	// Loads a static image that is displayed if the video cannot be played
	void setImage(std::string path);

	// Configures the component to show the default video
	void setDefaultVideo();
	
	virtual void onShow() override;
	virtual void onHide() override;
	virtual void onScreenSaverActivate() override;
	virtual void onScreenSaverDeactivate() override;
	virtual void topWindow(bool isTop) override;

	//Sets the origin as a percentage of this image (e.g. (0, 0) is top left, (0.5, 0.5) is the center)
	void setOrigin(float originX, float originY);
	inline void setOrigin(Eigen::Vector2f origin) { setOrigin(origin.x(), origin.y()); }

	void onSizeChanged() override;
	void setOpacity(unsigned char opacity) override;

	virtual void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties) override;

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

	// Returns the center point of the video (takes origin into account).
	Eigen::Vector2f getCenter() const;

	virtual void update(int deltaTime);

	void render(const Eigen::Affine3f& parentTrans) override;

protected:
	// Start the video Immediately
	virtual void startVideo() = 0;
	// Stop the video
	virtual void stopVideo() { };
	// Handle looping the video. Must be called periodically
	virtual void handleLooping();

	// Start the video after any configured delay
	void startVideoWithDelay();

	// Handle any delay to the start of playing the video clip. Must be called periodically
	void handleStartDelay();

	// Manage the playing state of the component
	void manageState();

protected:
	unsigned						mVideoWidth;
	unsigned						mVideoHeight;
	Eigen::Vector2f 				mOrigin;
	float							mFadeIn;
	std::string						mStaticImagePath;
	ImageComponent					mStaticImage;

	boost::filesystem::path			mVideoPath;
	boost::filesystem::path			mPlayingVideoPath;
	bool							mStartDelayed;
	unsigned						mStartTime;
	bool							mIsPlaying;
	bool							mShowing;
	bool							mDisable;
	bool							mScreensaverActive;

	Configuration					mConfig;
};

#endif