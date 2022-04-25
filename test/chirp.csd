;linear swept chirp
;adapted from http://www.csounds.com/journal/issue14/CsoundWorkbench.html
;use sin to minimize any interpolation artifacts
<CsoundSynthesizer>
<CsOptions>
csound -s -d -m0 -W -ochirp.wav -b800 -B800
</CsOptions>
<CsInstruments>
sr      = 48000
nchnls  = 1
0dbfs   = 32767

instr	1
  kfreq linseg  0.0, p3, 1.0
  aphs phasor sr/2 * kfreq
  aout sin aphs * $M_PI * 2
  out aout * 0dbfs
endin
</CsInstruments>
<CsScore>
; chirp 0-2 seconds
i1	0	2.00
e
</CsScore>
</CsoundSynthesizer>
