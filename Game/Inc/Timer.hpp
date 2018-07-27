/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

struct Timer final
{
	Timer()
	{
		auto desiredColor = TextColorCounting;
		auto startingColor = TextColorCountdown;
		auto t = CountdownTime.asSeconds();

		// just V = s/t
		colorDelta.r = ( desiredColor.r - startingColor.r ) / t;
		colorDelta.g = ( desiredColor.g - startingColor.g ) / t;
		colorDelta.b = ( desiredColor.b - startingColor.b ) / t;
		colorDelta.a = ( desiredColor.a - startingColor.a ) / t;
	}

	enum class Mode
	{
		Countdown,
		Counting
	} mode = Mode::Countdown;

	inline static const sf::Time CountdownTime = sf::seconds( 1 );
	sf::Clock clock;
	con::Text text;
	inline static const sf::Color TextColorCountdown = sf::Color::Red;
	inline static const sf::Color TextColorCounting = sf::Color::White;
	struct
	{
		float32_t r, g, b, a;
	} colorDelta;

	void reset()
	{
		auto countdownTimeStr = con::ConvertTo<std::string>( CountdownTime.asSeconds() + 0.001 );

		text.setString( con::ConvertTo<std::string>( "-", countdownTimeStr.substr( 0, countdownTimeStr.find( '.' ) + 3 ), "s" ) );
		clock.restart();
		mode = Mode::Countdown;
		text.setFillColor( TextColorCountdown );
	}

	void updateTimerText()
	{
		std::string secondsString;
		if ( mode == Mode::Countdown ) {
			float32_t sec = clock.getElapsedTime().asSeconds();
			float32_t countdownSec = CountdownTime.asSeconds();
			float32_t countdownDelta = countdownSec - sec;

			// @Bug: Can't set sf::Clock time point, so there is small amout of milliseconds that are discarded.
			if ( countdownDelta <= 0.0f ) {
				mode = Mode::Counting;
				clock.restart();
				text.setFillColor( TextColorCounting );
			} else {
				// Fading to TextColorCounting 
				auto currentColor = text.getFillColor();
				auto dt = con::Global.FrameTime.asSeconds();

				currentColor.r += colorDelta.r * dt;
				currentColor.g += colorDelta.g * dt;
				currentColor.b += colorDelta.b * dt;
				currentColor.a += colorDelta.a * dt;

				text.setFillColor( currentColor );

				secondsString = con::ConvertTo<std::string>( "-", countdownSec - sec );
			}
		}
		if ( mode == Mode::Counting )
			secondsString = con::ConvertTo<std::string>( clock.getElapsedTime().asSeconds() );

		auto finalString = secondsString.substr( 0, secondsString.find( '.' ) + 3 );
		text.setString( con::ConvertTo<std::string>( finalString, "s" ) );
	}
};
