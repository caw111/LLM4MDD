"""Explicit JSON requests and completion metadata shared by API and experiments."""


class JSONPrompt(str):
    """String-compatible request marker; never infer output mode from prose."""


def json_prompt(prompt):
    if isinstance(prompt, JSONPrompt):
        return prompt
    return JSONPrompt('Return one complete JSON object.\n'+str(prompt))


def request_options(prompt):
    return {'response_format': {'type': 'json_object'}} if isinstance(prompt, JSONPrompt) else {}


class CompletionText(str):
    def __new__(cls, content, *, finish_reason=None, refusal=None, response_format=None, usage=None):
        result = super().__new__(cls, content)
        result.completion = dict(finish_reason=finish_reason, refusal=refusal, response_format=response_format)
        result.usage = usage
        return result


def completion_text(response, prompt):
    choice = response.choices[0]
    usage = getattr(response, 'usage', None)
    if hasattr(usage, 'model_dump'):
        usage = usage.model_dump()
    elif usage is not None and not isinstance(usage, dict):
        usage = {key: getattr(usage, key) for key in ('prompt_tokens', 'completion_tokens', 'total_tokens', 'cost')
                 if getattr(usage, key, None) is not None}
    return CompletionText(choice.message.content or '', finish_reason=choice.finish_reason,
        refusal=getattr(choice.message, 'refusal', None), response_format=request_options(prompt).get('response_format'),
        usage=usage)


class JSONCompletionError(ValueError):
    def __init__(self, code, message):
        self.code = code
        super().__init__(message)


def check_completion(text):
    metadata = getattr(text, 'completion', {})
    if metadata.get('refusal'):
        raise JSONCompletionError('LLM_REFUSAL', 'Provider refused the structured response; no accepted artifact was produced')
    reason = metadata.get('finish_reason')
    if reason is not None and reason != 'stop':
        raise JSONCompletionError('JSON_INCOMPLETE', 'Structured response did not complete normally: finish_reason='+str(reason)+
            '. Preserve the full requirements; return a complete JSON object within the existing response budget.')
