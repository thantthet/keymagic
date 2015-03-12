# Introduction #

You can make keyboard layout with KeyMagic layout script language.

  * [Headers](LayoutScripting#Headers.md)
  * [Variables](LayoutScripting#Variables.md)
  * [Rules](LayoutScripting#Rules.md)
  * [VirtualKeys](LayoutScripting#Virtual_Key_Code.md)
  * [String Index](LayoutScripting#String_index.md)
  * [Using matched context in output](LayoutScripting#Using_matched_context_in_output.md)
  * [NULL Output](LayoutScripting#NULL_Output.md)
  * [Match Any](LayoutScripting#Match_Any.md)
  * [Not Of](LayoutScripting#Not_Of.md)
  * [Switch](LayoutScripting#Switch.md)
  * [Comments](LayoutScripting#Comments.md)
  * [Escape in string](LayoutScripting#Escape_in_string.md)
  * [Joining Lines](LayoutScripting#Joining_Lines.md)


# Details #

## Headers ##
**Description:** Headers are used to define keyboard layout descriptions, layout settings and icon path. And they needed to be in comment block.

**Syntax:** `@%name = %value`
```
/*
@Name = "A Smart Unicode Keyboard Layout"
@Description = "This is keyboard description"
@TRACK_CAPSLOCK = "false"
@EAT_KEYS = "true"
@US_LAYOUT_BASED = "true"
@SMART_BACKSPACE = "true"
@TREAT_CTRL_ALT_AS_RALT = "true"
*/
```

  * **Name:** Name of keyboard layout. `*` Implemented from v1.4
  * **Icon:** Full path or relative path of icon file. `*` Implemented from v1.4
  * **Description:** Description of the keyboard. `*` Implemented from v1.4
  * **TRACK\_CAPSLOCK:** Setting _false_ to this option will ignore caps locks key. (default: true)
  * **SMART\_BACKSPACE:** Backspace will acts as undo key that will restore to the most recent state before last user input. (default: false)
  * **EAT\_KEYS:** When this option is _true_, any undefined keys will not have any output. (default: false)
  * **US\_LAYOUT\_BASED:** Key code values will be based on US Keyboard Layout. (default: false)
  * **TREAT\_CTRL\_ALT\_AS\_RALT:** Pressing CTRL+ALT will be treat as RALT (ALT-Gr) (default: true)

**Note:** Options are case insensitive.

## Variables ##
**Dscription:** Variables are used to store strings.

**Syntax:** `$%name = %value`
```
$vowels_cap = "AEIOU"
```
```
$vowels_small = 'aeiou'
```
```
$ka = U1000
```
You can also use declared variables in new variable declaration.
```
$vowels = $vowels_cap + $vowels_small
```
You can use both String and Unicode in a variable.
```
$AAA = "A" + U0040 + $vowles
```

## Rules ##
**Description:** Rules are used to match the context what user type and how to customize the output.

**Syntax:** `%pattern_to_be_match => %output_pattern`
```
'a' => U1000
```
With the above rule, when user type _'a'_, the output will be _U1000_ (Myanmar က).

### Virtual Key Code ###
**Description:** Virtual keys are commonly used in LHS. Anyway, they could be used in the RHS too.

**Syntax:** `< %vkcode [ & %vkcode ] >`

%vkcode can be one of the VirtualCodes.
```
< VK_KEY_A > => U1000
```

The output will be _U1000_ (Myanmar က) when user type _'a'_.

Note: All Virtual Key  inside _<>_ need to be pressed at the same time to successfully match the pattern.
```
< VK_CTRL & VK_SHIFT & VK_KEY_A > => U1000
```
Above pattern will be matched only when the user press _CONTROL KEY_, _SHIFT KEY_ and _A KEY_.

### String index ###
**Description:** You can match one character from the string variable specifying index.

**Syntax:** `$var[%n]`

%n is the index of character.

```
$string = "ABCDE"
$string[1] => U1039
```

If you use `*` instead of index, it will be considered as any of character from the string array.
```
$string[*] => "You typed" + $1
```

When you used `*`, index of matched letter in LHS can be used in RHS.
```
$small = 'zxcv'
$cap = 'ZXCV'
$small[*] => $cap[$1]
// z to Z, x to X
$small[*] + $small[*] => $1 + $cap[$2]
// zx to zX, cv to cV
$small[*] + $cap[*] => $1 + $small[$2] + $2
// zZ to zzZ
```

### Using matched context in output ###
**Description:** You can use matched context of LHS in the RHS.

**Syntax:** `%context [ + %context ] => %n [ + %n]`

```
'will ' + 'this ' + 'swap' => $2 + $1 + $3
```
When the context is _**will this swap**_ then the output will be _**this will swap**_

### NULL Output ###
**Description:** Set the output to empty.

**Syntax:** `%context => null`
```
'Blacklis' + <VK_KEY_T> => NULL // Ex1
'I need this string to delete with single backspace' + <VK_BACK> => null // Ex2
```
Ex1: In this example, when the context is 'Blacklis' and typing 'T' will result in empty output.
Ex2: This will delete whole string as soon as you type backspace.

Note: You can still use '' or "" to have null output. `'abc' => '' `

### Match Any ###
**Description:** Use in the left side of the pattern. And used to match any character.

**Syntax:** `* [+ %context] => [%pattern]`
```
'i' + * => 'I' + $2
```

### Not Of ###
**Description:** Used with string arrays (variables). Will match any character except from the string array.

**Syntax:** `$var[^]`
```
$vowels = 'aeiou'
$vowels[^] => $1 + ' is not a vowel'
```

### Switch ###
**Description:** Switches are internal boolean(true/false) values. You could use switches as deadkey. After switch value is changed to **TRUE**, **TRUE** value only valid for next input.

**Syntax:** ('%switch\_name')
```
'=' => ('myswitch') // When user type '=', switch named myswitch's value is TRUE
'z' + ('myswitch') => 'zee' // myswitch's value is now FALSE and print 'zee'
'v' + ('myswitch') => 'vee' + ('myswitch') // myswitch's value is still TRUE and print 'vee'
```

### Comments ###
`//` : comment till the end of line.

`/* */`: comment multiline.

### Escape in string ###
```
$slash = '\\'
$d_quote = "\""
$aUnicode = "\u1000"
```

### Joining Lines ###
```
$vowels = 'aei' + \
          'ou'
$vowels[*] => \
            'vowles'
```