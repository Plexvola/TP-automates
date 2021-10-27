# TP-automates
tp automate

```
Regex 	-> Union Reste_R
Reste_R -> . Union Reste_R
		 | epsilon
Union 	-> Concat Reste_U
Reste_U -> + Concat Reste_U
		 | epsilon
Concat  -> * Kleene
Kleene  -> (Regex)
		 | ALPHABET
```

