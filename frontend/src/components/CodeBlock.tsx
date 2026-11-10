import { Copy, Download } from "lucide-react";
import { useMemo } from "react";
import toast from "react-hot-toast";
import { cn } from "../lib/cn";
import { downloadText } from "../lib/download";

export function CodeBlock(props: {
  title: string;
  summary?: string;
  content: string;
  filename?: string;
  className?: string;
}) {
  const trimmed = useMemo(() => props.content ?? "", [props.content]);

  return (
    <div
      className={cn(
        "overflow-hidden rounded-2xl border border-slate-200 bg-white shadow-[0_18px_50px_rgba(15,23,42,0.08)]",
        props.className,
      )}
    >
      <div className="flex items-center justify-between gap-3 border-b border-slate-200 bg-gradient-to-r from-white via-slate-50 to-cyan-50 px-4 py-3">
        <div className="min-w-0">
          <div className="truncate text-sm font-semibold text-slate-800">{props.title}</div>
          {props.filename ? <div className="truncate text-xs text-slate-500">{props.filename}</div> : null}
          {props.summary ? <div className="mt-1 text-xs text-cyan-700">{props.summary}</div> : null}
        </div>
        <div className="flex items-center gap-2">
          <button
            type="button"
            className="inline-flex items-center gap-2 rounded-lg border border-slate-200 bg-white px-3 py-1.5 text-xs font-medium text-slate-700 transition hover:border-cyan-300 hover:bg-cyan-50"
            onClick={async () => {
              await navigator.clipboard.writeText(trimmed);
              toast.success("Copied");
            }}
          >
            <Copy className="h-4 w-4" />
            Copy
          </button>
          {props.filename ? (
            <button
              type="button"
              className="inline-flex items-center gap-2 rounded-lg border border-slate-200 bg-white px-3 py-1.5 text-xs font-medium text-slate-700 transition hover:border-emerald-300 hover:bg-emerald-50"
              onClick={() => downloadText(props.filename!, trimmed)}
            >
              <Download className="h-4 w-4" />
              Download
            </button>
          ) : null}
        </div>
      </div>
      <pre className="max-h-[420px] overflow-auto bg-white px-5 py-4 font-mono text-[12px] leading-6 text-slate-800 whitespace-pre-wrap break-words">
        <code>{trimmed}</code>
      </pre>
    </div>
  );
}
