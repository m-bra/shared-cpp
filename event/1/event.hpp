
class Routine
{
    float _duration;
    float _last_event;

public:
    Routine(float duration):
	_duration(duration)
    {
	_last_event = 0;
    }

    template <typename F>
    void update(float deltatime, F const &on_event)
    {
	_last_event+= deltatime;

	if (_last_event > _duration)
	{
	    _last_event = 0;
	    on_event();
	}
    }

    void duration(float duration)
    {
	_duration = duration;
    }

    float duration()
    {
	return _duration;
    }
};
