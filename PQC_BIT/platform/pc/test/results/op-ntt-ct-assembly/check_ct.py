import json, pathlib, subprocess
root=pathlib.Path('/Users/john/work/PQC_Crypto')
out=pathlib.Path('/private/tmp/pqc-bit-ntt/ct');out.mkdir(exist_ok=True)
analyzer='/Users/john/.codex/plugins/cache/trailofbits/constant-time-analysis/0.2.6/ct_analyzer/analyzer.py'
compiler=subprocess.check_output(['clang','--version'],text=True).splitlines()[0]
records=[]
for arch in ('arm64','x86_64'):
 for opt in ('O2','O3'):
  for source in ('PQC_BIT/platform/pc/soft/op_ntt.c','PQC_BIT/core/common/ntt.c'):
   stem=pathlib.Path(source).stem
   asm=out/f'{stem}-{arch}-{opt}.s'
   cmd=['clang','-arch',arch,'-'+opt,'-std=c11','-IPQC_BIT/include','-IPQC_BIT/core/common','-IPQC_BIT/platform/pc','-S',source,'-o',str(asm)]
   subprocess.run(cmd,cwd=root,check=True,capture_output=True,text=True)
   result=subprocess.run(['python3',analyzer,'--assembly','--warnings','--json','--arch',arch,str(asm)],check=False,capture_output=True,text=True)
   report=json.loads(result.stdout)
   assert report['total_instructions']>0 and report['error_count']==0, report
   records.append({'source':source,'architecture':arch,'optimization':opt,'compiler':compiler,'command':cmd,'report':report})
   print(source,arch,opt,'errors',report['error_count'],'branch warnings',report['warning_count'])
summary={'scope':'New OP NTT software backend and adapter, static compiler inspection only','records':records,
 'triage':[
  'OP_ntt256_init branches validate public pointers, parameter sizes, q, inverse constants and k.',
  'OP_ntt256/OP_intt256 branches validate public ready-state/pointers/index, choose vectorized copies by public alias/alignment ranges, and iterate public 256-length/eight-layer butterfly loops.',
  'OP_cwm branches validate public length/q/k/pointers/ready-state and choose vectorized memory paths by public address ranges; coefficient loops have fixed public bounds.',
  'Adapter branches handle public operation kind, allocator/OP status and fixed loop bounds; coefficients are reduced with multiply-shift/masks, without secret division or coefficient-indexed table access.',
  'Raw tool warnings are retained; local assembly labels may be attributed as function names by the analyzer. These warning groups are triaged from source plus corresponding assembly, not treated as vulnerabilities or suppressed.',
  'No complete signing constant-time/security or hardware timing claim is established; only these compiler/architecture/optimization combinations were inspected.'
 ]}
(out/'report.json').write_text(json.dumps(summary,indent=2)+'\n')
