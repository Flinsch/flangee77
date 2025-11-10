
[![License](https://img.shields.io/github/license/Flinsch/flangee77)](https://github.com/Flinsch/flangee77/blob/main/LICENSE.md)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue?logo=C%2B%2B)](https://en.cppreference.com/w/cpp/20)
[![Build & Test](https://github.com/Flinsch/flangee77/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Flinsch/flangee77/actions/workflows/cmake-multi-platform.yml)
[![Documentation](https://github.com/Flinsch/flangee77/actions/workflows/documentation.yml/badge.svg)](https://github.com/Flinsch/flangee77/actions/workflows/documentation.yml)

# flangee77
*An unlikely modular game framework*

---

This framework is so unlikely that it probably shouldn't exist.
At least _you_ probably shouldn't use it at all!
I mean, you _can_ use it, of course. It's here, under the [MIT License](https://choosealicense.com/licenses/mit/). So go ahead!
But you would certainly be better off using other frameworks or even game engines, whether they are quadruple-A tier or free to use.

Before we go further, let's get something straight: this isn't a game engine.
If an engine is like a full-course meal (prepared, plated, and ready to serve), this framework is more like a set of some ingredients and a recipe book (a recipe book that is missing a few pages).
An engine offers everything you need, often at the cost of flexibility, as you're bound by its design and philosophy.
For most developers, that's perfectly fine: engines are powerful, mature, and cover almost any need you'll ever have, at least indirectly via possible workarounds, if not directly.

A framework, on the other hand, offers more freedom, but also requires more responsibility.
It's a structured set of tools that you can use, but you'll still need to roll up your sleeves and cook up your own solutions.
You're free to stay within the &ldquo;frame&rdquo; or step outside it when you need to, but there's no hand-holding or guarantees that everything you'll want is already there.

(To clear up any misunderstandings of a tired reader: enjoying the meal in the metaphor above means making a game, not playing it.)

Think of it this way: if engines are for those who want everything ready to go, and libraries are for those who want to piece everything together from scratch, a framework sits comfortably in the middle.
It gives you tools, structure, and flexibility, but the rest is up to you.

Okay, okay, but what exactly is all this about, you ask? Haha, good question!
I'll try to give a somewhat comprehensible answer to that, see my [Personal Background and Intention](docs/personal-intention.md).

And to answer the question of who might use this framework now, I will try to make a case distinction:

- Are you a **professional** video game developer? <br>
  Then you should definitely not use this framework and instead go for something really professional:
  robust, industry-standard tools that are tested, maintained, and supported by large teams, an off-the-shelf engine, or an in-house engine from your company.
  Compared to those, this framework is an absolute joke: it's highly unlikely to fit into your existing processes unless you want to ruin your company.
- Are you an **indie** video game developer? <br>
  Then you are likely focused on speed, polish, and tools that &ldquo;just work&rdquo;.
  You are interested in getting a finished game pretty soon and should use tools that help you with &ldquo;everything&rdquo; and not just an unusual framework where you still have to build a lot of things yourself.
  <br>(Of course, you can still be a professional as an indie developer, that's for sure. Please don't get me wrong here!)
- Are you a **hobbyist** video game developer? <br>
  Then the question is what your intention is:
  Do you really want to do game development that will produce a finished product?
  Then the same answer applies to you as to the indie developer.
  Or are you like me and prefer to build &ldquo;everything&rdquo; yourself because it's fun and the journey is the goal, not necessarily a finished game, even though that would still be welcome?
  Well, then the recommendation is not necessarily a full-fledged engine, but still another, mature framework.
  Or you can do what I did and build your own framework (or even your own engine).
- Are you the **original author** (that's me!)? <br>
  Then I'm just talking to myself.
  Let's be honest:
  This framework was made for my own &ldquo;creative&rdquo; needs, challenges, and ideas.
  It reflects my journey and priorities (see my [Personal Background and Intention](docs/personal-intention.md) again), and while it may not suit everyone, it's &ldquo;exactly&rdquo; what I wanted to build.
  (Well, not quite. This may sound familiar to you: when you look at your own code after a few months, you want to throw parts of it out and do it again in a new and better way. So this will never be &ldquo;stable&rdquo; or &ldquo;finished&rdquo; anyway.)

Now, I'm not here to start a big discussion about whether and when you should use which framework/engine or even build your own, but you see: there are &ldquo;not many&rdquo; reasons to use _this_ framework.
I've probably done a better job of convincing you to _not_ use it than the other way around.
But hey, maybe you're thinking, _what kind of framework is so unlikely that its own creator argues against using it? I have to try this precisely because of that!_

In this case, yes: Welcome, my friend!

---

## API Reference and Source Code

- Documentation: https://flinsch.github.io/flangee77
- GitHub: https://github.com/flinsch/flangee77

## License

The flangee77 framework is licensed under the general [MIT License](https://opensource.org/licenses/MIT/), which can be found specifically [here](LICENSE.md).
