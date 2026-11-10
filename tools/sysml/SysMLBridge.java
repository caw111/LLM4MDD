import java.nio.file.*;
import java.util.*;
import com.google.gson.Gson;
import org.eclipse.emf.ecore.*;
import org.eclipse.xtext.nodemodel.util.NodeModelUtils;
import org.omg.sysml.interactive.SysMLInteractive;
import org.omg.sysml.lang.sysml.Element;

/** JSON boundary around the unmodified OMG community reference implementation. */
public class SysMLBridge {
    static Object value(Object v) {
        if (v instanceof Element e) {
            Map<String,Object> ref = new LinkedHashMap<>();
            ref.put("id", e.getElementId());
            ref.put("name", e.getQualifiedName());
            ref.put("kind", e.eClass().getName());
            return ref;
        }
        if (v instanceof Collection<?> c) return c.stream().map(SysMLBridge::value).toList();
        if (v == null || v instanceof String || v instanceof Number || v instanceof Boolean) return v;
        return v.toString();
    }
    static Map<String,Object> element(Element e) {
        Map<String,Object> out = new LinkedHashMap<>();
        out.put("id", e.getElementId());
        out.put("kind", e.eClass().getName());
        out.put("name", e.getDeclaredName());
        out.put("qualifiedName", e.getQualifiedName());
        var node = NodeModelUtils.getNode(e);
        if (node != null) {
            out.put("line", node.getStartLine());
            out.put("text", node.getText());
        }
        Map<String,Object> fields = new LinkedHashMap<>();
        for (var f : e.eClass().getEAllStructuralFeatures()) {
            // Export semantic references as IDs, never recursively traverse library graphs.
            if (!f.isDerived() && !f.isTransient()) fields.put(f.getName(), value(e.eGet(f)));
        }
        for (String key : List.of("type", "direction", "featureTarget", "ownedMember", "result", "argument", "operator", "value", "relatedFeature", "referent", "targetArgument", "valueExpression", "thenAction", "elseAction", "ifArgument")) {
            var f = e.eClass().getEStructuralFeature(key);
            if (f != null) fields.put(key, value(e.eGet(f)));
        }
        if (e instanceof org.omg.sysml.lang.sysml.MultiplicityRange range) out.put("scalarMultiplicity", range.hasBounds(1, 1));
        out.put("fields", fields);
        return out;
    }
    public static void main(String[] args) throws Exception {
        if (args.length != 3) throw new IllegalArgumentException("library-dir input.sysml output.json");
        var engine = SysMLInteractive.createInstance();
        engine.loadLibrary(args[0]);
        var result = engine.process(Files.readString(Path.of(args[1])), false);
        Map<String,Object> output = new LinkedHashMap<>();
        output.put("ok", !result.hasErrors() && result.getException() == null);
        List<Object> issues = new ArrayList<>();
        if (result.getIssues() != null) for (var issue : result.getIssues()) {
            Map<String,Object> diagnostic = new LinkedHashMap<>();
            diagnostic.put("severity", issue.getSeverity().toString().toLowerCase());
            diagnostic.put("code", issue.getCode());
            diagnostic.put("message", issue.getMessage());
            diagnostic.put("line", issue.getLineNumber());
            diagnostic.put("column", issue.getColumn());
            diagnostic.put("syntax", issue.isSyntaxError());
            issues.add(diagnostic);
        }
        if (result.getException() != null) throw result.getException();
        output.put("issues", issues);
        List<Object> elements = new ArrayList<>();
        if (!result.hasErrors() && result.getRootElement() != null) {
            var root = result.getRootElement();
            elements.add(element(root));
            var iterator = root.eAllContents();
            while (iterator.hasNext()) {
                var next = iterator.next();
                if (next instanceof Element e) elements.add(element(e));
            }
        }
        output.put("elements", elements);
        Files.writeString(Path.of(args[2]), new Gson().toJson(output));
    }
}
