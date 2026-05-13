/** A flat list of peer tags. No hierarchy, no extra metadata. */
type DoxygenBlock = {
  tags: DoxygenTag[];
};

const ID_AND_TITLE_COMMANDS = [
  "defgroup",
  "section",
  "subsection",
  "subsubsection",
] as const;

const ID_COMMANDS = ["ingroup", "addtogroup"] as const;

const CONTENT_COMMANDS = [
  "brief",
  "author",
  "see",
  "note",
  "warning",
  "experimental",
  "attention",
  "important",
  "remark",
  "tip",
  "hint",
  "caution",
  "param",
  "return",
  "returns",
  "throws",
  "todo",
  "bug",
  "deprecated",
  "since",
] as const;

type IdAndTitleCommand = typeof ID_AND_TITLE_COMMANDS[number];
type IdCommand = typeof ID_COMMANDS[number];
type ContentCommand = typeof CONTENT_COMMANDS[number];
type DoxygenCommand = IdAndTitleCommand | IdCommand | ContentCommand;

const isIdAndTitleCommand = (s: string): s is IdAndTitleCommand =>
  (ID_AND_TITLE_COMMANDS as readonly string[]).includes(s);
const isIdCommand = (s: string): s is IdCommand =>
  (ID_COMMANDS as readonly string[]).includes(s);
const isContentCommand = (s: string): s is ContentCommand =>
  (CONTENT_COMMANDS as readonly string[]).includes(s);
const isDoxygenCommand = (s: string): s is DoxygenCommand =>
  isIdAndTitleCommand(s) || isIdCommand(s) || isContentCommand(s);

/** Fields shared by every tag variant. */
type TagMeta = {
  /** Line number (1-indexed) where the tag was found. */
  line: number;
  /** Original raw lines belonging to this tag. */
  raw: string;
};

/**
 * Each tag variant carries only the fields it actually uses; `command`
 * discriminates the union.
 */
type DoxygenTag =
  /** Free-floating prose with no preceding command. Synthetic — not a real Doxygen tag. */
  | (TagMeta & { command: "text"; content: string })
  /** `@defgroup`, `@section`, … — `<id> <title>` arguments, no body. */
  | (TagMeta & { command: IdAndTitleCommand; id?: string; title?: string })
  /** `@ingroup <id>` — single identifier argument, no body. */
  | (TagMeta & { command: IdCommand; id?: string })
  /** `@brief`, `@param`, … — commands whose argument is a block of text. */
  | (TagMeta & { command: ContentCommand; content: string });

export function doxygenParseAndRender(input: string): {
  blocks: DoxygenBlock[];
  markdown: string;
} {
  const blocks = doxygenParse(input);
  return { blocks, markdown: doxygenToMarkdown(blocks) };
}

/** Strip the common leading whitespace from a set of continuation lines. */
function dedent(lines: string[]): string[] {
  let minIndent = Infinity;
  for (const line of lines) {
    if (line.trim() === "") continue;
    const indent = line.length - line.trimStart().length;
    if (indent < minIndent) minIndent = indent;
  }
  if (minIndent === Infinity || minIndent === 0) return lines;
  return lines.map((l) => {
    const prefix = l.slice(0, minIndent);
    return /^\s*$/.test(prefix) ? l.slice(minIndent) : l;
  });
}

/**
 * Only `text` and content tags accumulate body lines; section and ingroup
 * tags are single-line and get pushed to the block directly.
 */
type PendingTag = {
  tag: Extract<DoxygenTag, { content: string }>;
  inline: string | null;
  contentBuf: string[];
  rawBuf: string[];
};

export function doxygenParse(input: string): DoxygenBlock[] {
  const lines = input.split(/\r?\n/);
  const blocks: DoxygenBlock[] = [];
  let block: DoxygenBlock = { tags: [] };
  let pending: PendingTag | null = null;

  const flushPending = () => {
    if (!pending) return;
    const continuation = dedent(pending.contentBuf);
    const parts = pending.inline !== null
      ? [pending.inline, ...continuation]
      : continuation;
    pending.tag.content = parts.join("\n").replace(/^\n+|\s+$/g, "");
    pending.tag.raw = pending.rawBuf.join("\n");
    block.tags.push(pending.tag);
    pending = null;
  };

  const endBlock = () => {
    flushPending();
    if (block.tags.length > 0) {
      blocks.push(block);
      block = { tags: [] };
    }
  };

  for (const [index, line] of lines.entries()) {
    const lineNo = index + 1;
    const m = line.match(/^\s*[@\\](\w+)\b\s*(.*)$/);
    const command = m?.[1];

    if (command && isDoxygenCommand(command)) {
      flushPending();
      const rest = m![2].trim();

      if (isIdAndTitleCommand(command)) {
        const [id, ...titleParts] = rest.split(/\s+/);
        block.tags.push({
          command,
          id: id || undefined,
          title: titleParts.join(" ") || undefined,
          line: lineNo,
          raw: line,
        });
      } else if (isIdCommand(command)) {
        block.tags.push({
          command,
          id: rest || undefined,
          line: lineNo,
          raw: line,
        });
      } else {
        pending = {
          tag: { command, content: "", line: lineNo, raw: "" },
          inline: rest || null,
          contentBuf: [],
          rawBuf: [line],
        };
      }
    } else if (line.trim() === "") {
      // Blank line ends the current tag's scope and closes the block.
      endBlock();
    } else {
      // Continuation line, or free-floating prose that opens a `text` tag.
      if (!pending) {
        pending = {
          tag: { command: "text", content: "", line: lineNo, raw: "" },
          inline: null,
          contentBuf: [],
          rawBuf: [],
        };
      }
      pending.contentBuf.push(line);
      pending.rawBuf.push(line);
    }
  }

  endBlock();
  return blocks;
}

export function doxygenToMarkdown(blocks: DoxygenBlock[]): string {
  return blocks.flatMap((block) =>
    block.tags.map((tag) => {
      switch (tag.command) {
        case "text":
          return tag.content;

        case "defgroup":
          return `# ${tag.title ?? tag.id ?? ""}` +
            (tag.id ? ` <a id="${tag.id}"></a>` : "");
        case "section":
          return `## ${tag.title ?? ""}` +
            (tag.id ? ` <a id="${tag.id}"></a>` : "");
        case "subsection":
          return `### ${tag.title ?? ""}` +
            (tag.id ? ` <a id="${tag.id}"></a>` : "");
        case "subsubsection":
          return `#### ${tag.title ?? ""}` +
            (tag.id ? ` <a id="${tag.id}"></a>` : "");

        case "ingroup":
        case "addtogroup":
          return null;

        case "brief":
          return `**${tag.content.trim()}**`;
        case "author":
          return `*Author:* ${tag.content}`;
        case "see":
          return `**See also:** \`${tag.content}\``;
        case "param":
          return `- **Param:** ${tag.content}`;
        case "return":
        case "returns":
          return `**Returns:** ${tag.content}`;
        case "throws":
          return `**Throws:** ${tag.content}`;
        case "since":
          return `*Since:* ${tag.content}`;

        case "attention":
        case "bug":
        case "caution":
        case "deprecated":
        case "experimental":
        case "hint":
        case "important":
        case "note":
        case "remark":
        case "tip":
        case "todo":
        case "warning":
          return `:::${tag.command}\n${tag.content}\n:::`;
      }
    })
  ).filter((item) => item !== null).join("\n\n");
}
