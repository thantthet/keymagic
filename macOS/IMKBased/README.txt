
InputMethodKit Backporting Component, or IMK-Tiger for short, helps
input method developers backport their IMK-based input method apps to
OS X 10.4 Tiger. It is what we use to backport the latest OpenVanilla, 
a popular input method toolset in Taiwan, to 10.4.

You can find the project at:

  http://code.google.com/p/imk-tiger/
  
A summary:

OS X 10.5 introduced InputMethodKit (IMK), arguably the best OS-level
input method framework in the industry. IMK is pure Cocoa and greately
simplifies input method development with a client-server design, powered by
Objective-C's distributed object mechanism. Input method developers only need
to talk to a very slim and object-oriented interface to provide input method
service to OS X. This allows developers to concentrate on algorithms and
user experience. On other platforms, OS X 10.4 included, input method
development involves many platform-specific details and debugging headaches
that can be a big distraction.

There is a catch--IMK is Leopard-only. In a market like Taiwan, where
Traditional Chinese is the main written language, there are still 45%-55%
of users having their Tiger machines around, and this is a market
we want to continue supporting.

What if we could replicate IMK on Tiger? In fact we can and have. IMK-Tiger
is the result. We're now working on an IMK-based OpenVanilla, and using
IMK-Tiger to build the Tiger version.

Please find the details in the project home mentioned above. We have supplied
a sample project, with a working, IMK-based input method ("SimpleIME") and
necessary project settings to produce a TSM-based counterpart.

Note that we call it a backporting component, not a library, because it is not
as trivial as linking to a library and voilà. But the crux is to customize
IMK-Tiger's ComponentConfig.h so that the TSM component provided can 
establish connection to your IMK server process (and launch it if it hasn't).
But once you have finished knitting everything together, you can concentrate
on improving your IMK-based code.

For now IMK-Tiger doesn't support -[IMKInputController recognizedEvents:]
and -[IMKInputController currentKeyboardLayout], and you must implement
-[IMKInputController handleEvent:client:] to handle the keyboard event.

IMK-Tiger is open source released under the BSD License. Feedback and
participation are welcome and appreciated.

Thanks!


