
# Personal Background and Intention

Since I was a young teenager, I've always wanted to be a video game developer.
Unfortunately, I never managed to turn that into a career.
However, maybe that's for the best, because I think the day-to-day of video game development, with its economic pressures, can be pretty tough.
What I have managed to do, though, is become a professional software developer in general.
Video game development has since remained one of my hobbies.
And since it's not a professional thing, it stays a hobby&mdash;one that I can enjoy with all the facets that interest me, and without the aspects that I don't feel like doing anyway.

Since it's a hobby, for me, the journey is the goal, not the finished product.
Others might feel differently, but that's how I see it.
That's also why I don't use off-the-shelf frameworks or even full-fledged engines;
I prefer to program &ldquo;everything&rdquo; myself.
I'm primarily interested in the technical, low-level stuff anyway, so
I usually keep experimenting with tricky things as prototypes rather than actually releasing a finished product.
And at the end of the day, I'm a programmer, not a game designer&mdash;a realization I've come to more than once.

Added to this is a mix of curiosity, stubbornness, and the sheer joy of solving problems in unconventional ways.
While most developers might choose polished, off-the-shelf, battle-tested tools,
this framework stands apart as a deeply personal experiment: a modular, multi-faceted toolset for creating (prototypical aspects of) games in ways you might not expect.

## The Journey of (Re)building My Framework

It must have been around 2005 when I laid the foundation for this framework.
Well, that's not entirely accurate.
It was actually a predecessor of this framework that I (further) developed and used for about 18 years.
Up until then, I had exclusively targeted DirectX with Direct3D 9 on Windows, limited to 32-bit.
But in the middle of 2023, I suddenly decided to rewrite my framework from scratch because I was running into issues with the existing 32-bit implementation based on Direct3D 9 under Windows 11.

It would probably have been enough to abandon the DirectX SDK (June 2010) and switch to the long-recommended Windows SDK, which would have meant more than just replacing a few includes and libraries, and perhaps optionally switching from 32-bit to 64-bit.
Well, in the end, I did make the switch&mdash;both of those changes&mdash;but with the far more consequential decision to rebuild &ldquo;everything&rdquo; from the ground up (well, I still reuse a lot of things, of course).
This time, I aimed to generalize the framework so it wouldn't be limited to a single technology stack but could potentially support multiple ones (e.g., Direct3D 9 and Direct3D 11, and maybe later OpenGL or others, and generally not just DirectX components).

This, of course, requires an additional abstraction layer, as the technologies offer different interfaces while the application is meant to offer a uniform &ldquo;facade.&rdquo;
And who knows? Perhaps I'll even target platforms other than Windows. We'll see.
But smartphones, and especially consoles, are probably off the table&mdash;that wouldn't make much sense, in my opinion, even despite (or perhaps because of) my personal background I've just described.
