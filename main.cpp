#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

double frequency = 440.;
double increment = 440./44100;
sf::Sound Sound;

void func()
{
	const unsigned SAMPLES = 44100;
	const unsigned SAMPLE_RATE = 44100;
	const unsigned AMPLITUDE = 30000;
	
	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;

	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sin(x*TWO_PI);
		x += increment;
	}
	
	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		//return 1;
	}


	Sound.setBuffer(Buffer);
	Sound.setLoop(true);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}
}

class CustomStream : public sf::SoundStream
{
private:
    sf::Int16 * minChunk;
    size_t last;
public:
    bool open(const std::string& location)
    {
        // Open the source and get audio settings
        unsigned int channelCount = 1;
        unsigned int sampleRate = 44100;
        // Initialize the stream -- important!
        initialize(channelCount, sampleRate);
        last = 0;
        generate();
        return true;
    }

    void generate(){
        const unsigned SAMPLES = 44100*2;
        const unsigned SAMPLE_RATE = 44100;
        const unsigned AMPLITUDE = 30000;
        
        sf::Int16 raw[SAMPLES];

        const double TWO_PI = 6.28318;

        double x = 0;
        for (unsigned i = 0; i < SAMPLES; i++) {
            raw[i] = AMPLITUDE * sin(x*TWO_PI);
            x += increment;
        }

        minChunk = raw;
        last = 0;
    }
    void regenerate(){
        const unsigned SAMPLES = 44100*2;
        const unsigned SAMPLE_RATE = 44100;
        const unsigned AMPLITUDE = 30000;

        const double TWO_PI = 6.28318;

        double x = 0;
        for (unsigned i = 0; i < SAMPLES; i++) {
            minChunk[i] = AMPLITUDE * sin(x*TWO_PI);
            x += increment;
        }
        last = 0;
    }
private:
    bool onGetData(Chunk& data)
    {
        // Fill the chunk with audio data from the stream source
        // (note: must not be empty if you want to continue playing)




/*
        const unsigned SAMPLES = 44100;
        const unsigned AMPLITUDE = 500;
        sf::Int16 raw[SAMPLES];
        int m = 1;
        int c = 0;
        for ( unsigned i = 0; i < SAMPLES; i++ ){
            raw[i] = m * AMPLITUDE;
            c++;
            if ( c == 100 ){
                c = 0;
                m = -m;
            }
        }
*/

        data.samples = minChunk + last;
        data.sampleCount = 2205;//882*5;
        //last += 2205;
        //last += 2205;

/*
        if ( last > 0 ){
            double step = 1./increment;
            step *= 2.;
            int st = step;
            while ( last - st > 0 ){
                last -= st;
            }
        }
*/

        //last = last % 200;

        // Return true to continue playing
        return true;
    }
    void onSeek(sf::Time timeOffset)
    {
        std::cout << "On seek" << std::endl;
    }
};


int main() {
    //sf::Thread thread(&func);
    //thread.launch();


    CustomStream stream;
    stream.open("path/to/stream");
    stream.play();

/*
    while(true){
        float temp;
        std::cin >> temp;
        Sound.setVolume(temp);
    }
*/

/*
    while(true){
        float temp;
        std::cin >> temp;
        increment = temp/44100;
        stream.regenerate();
    }
*/

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed){
                //stream.setVolume(100.);
                std::cout << "HELLO " << event.key.code << std::endl;

                const double BASE_FREQ = 220.;
                double freq = BASE_FREQ;
                for ( int i = 0; i < event.key.code; i++ ){
                    freq *= 1.05946309436;
                }
                //freq = BASE_FREQ + 40. * event.key.code;
                //frequency = frequency * 1.05946309436;
                increment = freq/44100;

                double step = 1./increment;
                step *= 2.;
                int st = step;
                std::cout << step << " " << st << std::endl;
                stream.regenerate();
            }
            if(event.type == sf::Event::KeyReleased){
                //stream.setVolume(0);
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }


/*
        const unsigned SAMPLES = 44100;
        const unsigned SAMPLE_RATE = 44100;
        const unsigned AMPLITUDE = 30000;
        
        sf::Int16 raw[SAMPLES];

        const double TWO_PI = 6.28318;

        double x = 0;
        std::cout <<  AMPLITUDE * sin(x*TWO_PI) << std::endl;
        for (unsigned i = 0; i < SAMPLES; i++) {
            raw[i] = AMPLITUDE * sin(x*TWO_PI);
            x += increment;
        }
        std::cout << raw[SAMPLES-1] << std::endl;
*/
	return 0;
}