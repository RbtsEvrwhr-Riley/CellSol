---
publishdate: 2019-11-17
lastmod: 2020-11-24
---

Compression is a useful tool for stretching low bandwidth connections a bit further, but comes with significant computing overhead. Using dictionary compression attempts to remove the overhead of traditional
compression algorithms, while retaining their effectiveness for a specific subset of data - notably, large blocks of text.

### Why might we want word compression for our LoRa network?

The "commercial code" below is one of several methods that one can use to achieve excellent "word compression" for english words. "Word compression" is a concept that originated during the old days of telegrams and systems where you paid for every word to be transmitted. In our case, every word counts because our network bandwidth is so limited - and is literally as slow as the old dial-up modems of the 90s. (As slow as 9,600 - 12,000 baud modems)
These codes are examples of what is called "dictionary compression." An application is written in such a way that it recognizes common phrases, like "what time should we meet?" and assigns it a code that will necessitate transmitting fewer characters.

The way it works: The application (in this case our texting app & bulletin board app) - recognizes the phrase the person has typed and then uses a shorter abbreviation to represent it - sends the code (that uses less characters) across the network - where it is decoded on the receiver's side into the original message. This is similar to the autocorrect functions that everyone is used to using on their phones.

### What would creating our own codes involve?

Creating our own codes would involve determining the 32768 most common words in english - perhaps algorithmically.

### Why don't we use lossless text compression then instead of codes?
Because compression not optimized for text doesn't go very far for short messages. To demonstrate this concept, one can take any one sentence typed here, save it as a text file, zip the text file, and see that it won't be much shorter.

### Why are we discussing this for this project?
(This subject came up because there's a big emphasis in sending as few bytes as possible on the LoRa discussion forums.)
Let's say that we take the most common 32768 words in the english language (the other 32767 are pairs of ASCII characters that disallow control characters, so losing one bit out of 16 i not a big problem). Now each word, or even common phrase, if we want, fits two bytes. You can even do a bit of cleanup so that "how are you" and "how r u" and so on map to the same thing, and are printed "how are you" at the receiving end unless tagged "verbatim." [? (Note: we haven't discussed tagging messages? let's! :) ?]

Note: This is done phone/pc side where you have a lot of hard disk space for a dictionary and a lot of cpu power for realtime substitution.

Note: Interestingly, you can even XOR the -dictionary- with a crypto key for private messaging, and if you swap those often enough, that's your encryption solution. (Not unbeatable of course, but since it can be of arbitrary length, you could even have a one time pad, which IS unbeatable by definition). This type of "encryption" won't slow things down as much, as it's just a XOR operation that you do once on the dictionary. So, not unbeatable, but decent. And not super secure in that it requires a shared key, but eh.

### Current Implementation

We have no implementation currently used by our firmware, but we have included some example implementations in our GitHub repository. Smaz, PySmaz, Shoco, and a dictionary compression algorithm designed by Kay Borri are all included.

We encourage compression to be implemented on the client side of any CellSol-integrated application, rather than along the network itself, in order to maintain as much intercompatibility as possible.

### Commercial code (communications)
https://en.wikipedia.org/wiki/Commercial_code_(communications)
"In telecommunication, a commercial code is a code once used to save on cablegram costs.[1] Telegraph (and telex) charged per word sent, so companies which sent large volumes of telegrams developed codes to save money on tolls."